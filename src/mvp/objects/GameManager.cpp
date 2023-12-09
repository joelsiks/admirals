#include "objects/GameManager.hpp"
#include "events/EventArgs.hpp"
#include "objects/MenuManager.hpp"
#include "objects/NetworkManager.hpp"

using namespace admirals::mvp::objects;

GameManager::GameManager(const std::string &name, const Texture &atlas)
    : GameObject(name), m_atlas(atlas) {
    m_networkManager = GameData::engine->MakeGameObject<NetworkManager>(
        "networkManager", (*this));
    m_menuManager =
        GameData::engine->MakeGameObject<MenuManager>("menuManager", (*this));
}

GameManager::~GameManager() {}

void GameManager::OnStart(const EngineContext &) { CreateBases(); }

void GameManager::OnUpdate(const EngineContext &) {}

bool GameManager::StartAndConnectToServer(uint16_t port,
                                          const size_t maxTries) {
    return m_networkManager->StartAndConnectToServer(port, maxTries);
}

bool GameManager::ConnectToServer(const std::string &ip, uint16_t port,
                                  const size_t maxTries) {
    return m_networkManager->ConnectToServer(ip, port, maxTries);
}

void GameManager::StartGame() {
    m_gameStarted = true;
    m_waitingForOpponent = false;
}

void GameManager::StopGame(uint8_t winner) {
    if (!m_gameStarted) {
        return;
    }
    m_gameStarted = false;
    m_menuManager->ToggleEndGameMenu(winner == m_playerId);
    if (m_debug) {
        printf("Game stopped\n");
    }
}

void GameManager::ReadyUp() {
    if (m_gameStarted) {
        return;
    }
    m_waitingForOpponent = true;
    m_networkManager->ReadyUp();
}

// Called when the server disconnects
void GameManager::AbortGame() {
    if (!m_gameStarted && !m_waitingForOpponent) {
        return;
    }
    m_gameStarted = false;
    m_waitingForOpponent = false;
    m_menuManager->ToggleServerDisconnectMenu();
    if (m_debug) {
        printf("Game aborted\n");
    }
}

void GameManager::PauseGame() {
    m_gamePaused = true;
    m_menuManager->ToggleOpponentDisconnectMenu();
}

void GameManager::ResumeGame() {
    if (m_gamePaused)
        m_menuManager->ToggleOpponentDisconnectMenu();
    m_gamePaused = false;
    m_gameStarted = true;
}

void GameManager::CreateBases() {
    const Vector2 cellSize = Vector2(GameData::CellSize);
    const auto baseTop = GameData::engine->MakeGameObject<Base>(
        "baseTop", m_atlas, 2,
        Rect(0, 1, GameData::CellSize, GameData::CellSize));

    const auto baseBottom = GameData::engine->MakeGameObject<Base>(
        "baseBottom", m_atlas, 2,
        Rect(static_cast<float>(GameData::GridCells) - 1,
             static_cast<float>(GameData::GridCells) - 2, GameData::CellSize,
             GameData::CellSize));
    m_baseTop = baseTop;
    m_baseBottom = baseBottom;
}

void GameManager::BuyShip(uint8_t type) {
    if (m_coins < ShipInfoMap[type].Cost) {
        return;
    }

    m_networkManager->BuyShip(type);
}

void GameManager::MoveShip(uint16_t id, uint8_t x, uint8_t y) {
    if (m_ships.find(id) == m_ships.end() ||
        m_ships[id]->GetPlayerId() != m_playerId) {
        return;
    }

    m_networkManager->MoveShip(id, x, y);
}

void GameManager::AttackShip(uint16_t id, uint16_t targetId) {
    if (m_ships.find(id) == m_ships.end() ||
        m_ships.find(targetId) == m_ships.end() ||
        m_ships[id]->GetPlayerId() != m_playerId) {
        return;
    }
    printf("AttackShip\n");
    m_networkManager->AttackShip(id, targetId);
}

void GameManager::ShipChangeEventHandler(void *sender,
                                         admirals::events::EventArgs) {
    const Ship *ship = static_cast<Ship *>(sender);
    switch (ship->GetAction()) {
    case ShipAction::Move: {
        MoveShip(ship->GetID(), ship->GetActionX(), ship->GetActionY());
        break;
    }
    case ShipAction::Attack:
        AttackShip(ship->GetID(), ship->GetAttackID());
        break;
    default:
        break;
    }
}

void GameManager::UpdateBoard(int turn, int coins, int baseHealth,
                              int enemyBaseHealth,
                              const std::map<uint16_t, ShipData> &ships) {
    if (coins != m_coins) {
        CoinsChangedEventArgs e = CoinsChangedEventArgs(coins);
        onCoinsChanged.Invoke(this, e);
        m_coins = coins;
    }
    const auto playerBase = m_isTopPlayer ? m_baseTop : m_baseBottom;
    const auto enemyBase = m_isTopPlayer ? m_baseBottom : m_baseTop;
    if (m_baseHealth != baseHealth) {
        m_baseHealth = baseHealth;
        if (playerBase != nullptr) {
            playerBase->SetHealth(static_cast<float>(baseHealth));
        }
    }
    if (m_enemyBaseHealth != enemyBaseHealth) {
        m_enemyBaseHealth = enemyBaseHealth;
        if (enemyBase != nullptr) {
            enemyBase->SetHealth(static_cast<float>(enemyBaseHealth));
        }
    }
    m_turn = turn;
    ModifyShips(ships);
}

void GameManager::PlayAgain() {
    ResetState(false);
    m_menuManager->ToggleEndGameMenu();
    ReadyUp();
}

void GameManager::ExitToMenu() {
    // m_networkManager->Disconnect();
    ResetState(true);
    m_menuManager->ReturnToMenu();
}

void GameManager::ResetState(bool removeConnection) {
    m_gameStarted = false;
    m_gamePaused = false;
    m_waitingForOpponent = false;

    m_turn = 0;
    m_coins = 0;
    m_shipsP1 = 0;
    m_shipsP2 = 0;
    m_baseHealth = 0;
    m_enemyBaseHealth = 0;
    for (const auto &[_, ship] : m_ships) {
        GameData::engine->GetScene()->RemoveDisplayable(ship->identifier());
    }

    GameData::engine->GetScene()->RemoveDisplayable("baseTop");
    GameData::engine->GetScene()->RemoveDisplayable("baseBottom");
    CreateBases();

    m_ships.clear();

    if (removeConnection) {
        GameData::engine->GetScene()->RemoveDisplayable("networkManager");
        m_networkManager = GameData::engine->MakeGameObject<NetworkManager>(
            "networkManager", (*this));
        m_playerId = 0;
        m_isTopPlayer = false;
    }
}

void GameManager::ModifyShips(const std::map<uint16_t, ShipData> &ships) {
    std::unordered_set<uint16_t> keepSet;
    for (const auto &[_, ship] : ships) {
        if (ship.id == 0) {
            continue;
        }

        keepSet.insert(ship.id);

        auto it = m_ships.find(ship.id);

        if (it != m_ships.end()) {
            it->second->SetPosition(ship.x, ship.y);
            it->second->SetHealth(ship.health);
            it->second->SetOwner(ship.owner);
            it->second->SetAction(ship.action);
        } else {
            if (m_debug)
                printf("Creating ship %d\n", ship.id);
            const std::shared_ptr<Ship> newShip =
                GameData::engine->MakeGameObject<Ship>(ship, m_cellSize,
                                                       m_atlas);

            newShip->onChanged +=
                BIND_EVENT_HANDLER(GameManager::ShipChangeEventHandler);
            m_ships[ship.id] = newShip;
        }
    }

    // Remove ships in m_ships that are not in ships
    for (auto it = m_ships.begin(); it != m_ships.end();) {
        const auto ship = it->second;
        if (!keepSet.contains(ship->GetID())) {
            if (m_debug)
                printf("Removing ship %d\n", ship->GetID());
            GameData::engine->GetScene()->RemoveDisplayable(
                it->second->identifier());
            it = m_ships.erase(it);
        } else {
            it++;
        }
    }
}
