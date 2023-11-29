#include "GameManager.hpp"
#include "events/EventArgs.hpp"
#include "objects/NetworkManager.hpp"

using namespace admirals::mvp::objects;

GameManager::GameManager(const std::string &name) : scene::GameObject(name) {
    m_networkManager = GameData::engine->MakeGameObject<NetworkManager>(
        "networkManager", (*this));
}

GameManager::~GameManager() {}

void GameManager::OnStart(const EngineContext &) { srand(time(NULL)); }

void GameManager::OnUpdate(const EngineContext &) {}

bool GameManager::StartAndConnectToServer(uint16_t port,
                                          const size_t maxTries) {
    return m_networkManager->StartAndConnectToServer(port, maxTries);
}

bool GameManager::ConnectToServer(const std::string &ip, uint16_t port,
                                  const size_t maxTries) {
    return m_networkManager->ConnectToServer(ip, port, maxTries);
}

void GameManager::StopGame() {
    m_gameStarted = false;
    m_ships.clear();
    if (m_debug) {
        printf("Game stopped\n");
    }
}

void GameManager::BuyShip(uint8_t type) {
    if (m_coins < ShipInfoMap[type].Cost) {
        return;
    }

    m_networkManager->BuyShip(type);
}

void GameManager::MoveShip(uint16_t id, int x, int y) {
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
    // case ShipAction::Attack:
    //     break;
    default:
        break;
    }
}

void GameManager::UpdateBoard(int turn, int coins, int baseHealth,
                              int enemyBaseHealth,
                              const std::map<uint16_t, ShipData> &ships) {
    if (coins != m_coins) {
        CoinsChangesEventArgs e = CoinsChangesEventArgs(coins);
        onCoinsChanged.Invoke(this, e);
        m_coins = coins;
    }
    m_turn = turn;
    m_baseHealth = baseHealth;
    m_enemyBaseHealth = enemyBaseHealth;
    ModifyShips(ships);
}

void GameManager::ModifyShips(const std::map<uint16_t, ShipData> &ships) {
    for (const auto &[_, ship] : ships) {
        if (ship.id == 0) {
            continue;
        }

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
        const uint16_t id = it->first;

        if (ships.find(id) == ships.end()) {
            if (m_debug) {
                printf("Removing ship %s\n", it->second->name().c_str());
            }
            it = m_ships.erase(it);
        } else {
            it++;
        }
    }
}
