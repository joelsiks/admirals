#include "managers/GameManager.hpp"
#include "events/EventArgs.hpp"
#include "managers/MenuManager.hpp"
#include "managers/NetworkManager.hpp"
#include "objects/Base.hpp"

using namespace admirals::mvp::objects;

GameManager::GameManager(const std::string &name, const Texture &atlas,
                         bool debug)
    : GameObject(name, -10.f), m_atlas(atlas), m_debug(debug) {
    m_networkManager = GameData::GameScene->MakeGameObject<NetworkManager>(
        "networkManager", (*this), debug);
    m_menuManager = GameData::GameScene->MakeGameObject<MenuManager>(
        "menuManager", (*this));
}

GameManager::~GameManager() {}

void GameManager::OnStart(const EngineContext &) {}

void GameManager::OnUpdate(const EngineContext &) {
    // Reset navMesh to have it updated
    GameData::navMesh = nullptr;
}

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
    m_menuManager->ShowGameMenu();
}

void GameManager::StopGame(uint8_t winner) {
    if (!m_gameStarted) {
        return;
    }
    m_gameStarted = false;
    m_menuManager->ShowGameEndMenu(winner == m_playerId);
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
    m_menuManager->ShowWaitingMenu();
}

// Called when the server disconnects
void GameManager::AbortGame() {
    if (!m_gameStarted && !m_waitingForOpponent) {
        return;
    }
    m_gameStarted = false;
    m_waitingForOpponent = false;
    m_menuManager->ShowServerDisconnectMenu();
    if (m_debug) {
        printf("Game aborted\n");
    }
}

void GameManager::PauseGame() {
    m_gamePaused = true;
    m_menuManager->ShowOpponentDisconnectMenu();
}

void GameManager::ResumeGame() {
    if (m_gamePaused) {
        m_menuManager->ShowGameMenu();
    }
    m_gamePaused = false;
    m_gameStarted = true;
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

void GameManager::UpdateBoard(int turn, int coins,
                              const std::map<uint16_t, ShipData> &ships) {
    if (coins != m_coins) {
        CoinsChangedEventArgs e = CoinsChangedEventArgs(coins);
        onCoinsChanged.Invoke(this, e);
        m_coins = coins;
    }
    m_turn = turn;
    ModifyShips(ships);
}

void GameManager::PlayAgain() {
    m_menuManager->ShowGameMenu();
    ResetState(false);
    ReadyUp();
}

void GameManager::ExitToMenu() {
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

    m_ships.clear();
    GameData::Selection->Clear();

    if (removeConnection) {
        m_networkManager->StopServer();
        m_networkManager->Disconnect();
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
            it->second->SetPosition(static_cast<float>(ship.location.x),
                                    static_cast<float>(ship.location.y));
            it->second->SetHealth(ship.health);
            it->second->SetOwner(ship.owner);
            it->second->SetAction(ship.action);
        } else {
            if (m_debug) {
                printf("Creating ship %d\n", ship.id);
            }

            std::shared_ptr<Ship> newShip;
            if (ship.type == ShipType::Base) {
                newShip = GameData::engine->MakeGameObject<Base>(
                    ship, m_cellSize, m_atlas);
            } else {
                newShip = GameData::engine->MakeGameObject<Ship>(
                    ship, m_cellSize, m_atlas);
            }

            newShip->onChanged +=
                BIND_EVENT_HANDLER(GameManager::ShipChangeEventHandler);
            m_ships[ship.id] = newShip;
        }
    }

    // Remove ships in m_ships that are not in ships
    for (auto it = m_ships.begin(); it != m_ships.end();) {
        const auto ship = it->second;
        if (!keepSet.contains(ship->GetID())) {
            if (m_debug) {
                printf("Removing ship %d\n", ship->GetID());
            }

            GameData::engine->GetScene()->RemoveDisplayable(
                it->second->identifier());
            it = m_ships.erase(it);
        } else {
            it++;
        }
    }
}
