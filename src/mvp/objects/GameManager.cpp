#include "GameManager.hpp"
#include "NetworkManager.hpp"
#include "events/EventArgs.hpp"

using namespace admirals::mvp::objects;

GameManager::GameManager(const std::string &name)
    : scene::GameObject(name, 0, Vector3(0)) {
    m_networkManager = GameData::engine->MakeGameObject<NetworkManager>(
        "networkManager", (*this));
}

GameManager::~GameManager() {}

void GameManager::OnStart() { srand(time(NULL)); }

void GameManager::OnUpdate() {
    if (!m_gameStarted) {
        return;
    }

    // Tests for actions
    // if (m_turn % 10 == 5 && !m_testActionDone) {
    //     m_networkManager->BuyShip(ShipType::Destroyer);
    //     m_testActionDone = true;
    // }

    // if (m_turn % 10 == 0 && m_testActionDone) {
    //     auto it = std::find_if(
    //         m_ships.begin(), m_ships.end(),
    //         [this](const std::pair<uint16_t, std::shared_ptr<Ship>> &pair) {
    //             return pair.second->GetPlayerId() == m_playerId;
    //         });

    //     uint16_t id = it->second->GetId();

    //     m_networkManager->MoveShip(id, rand() % 10, rand() % 10);
    //     m_testActionDone = false;
    // }
}

void GameManager::StopGame() {
    m_gameStarted = false;
    m_ships.clear();
    if (m_debug) {
        printf("Game stopped\n");
    }
}

void GameManager::BuyShip(uint8_t type) { m_networkManager->BuyShip(type); }

void GameManager::MoveShip(uint16_t id, int x, int y) {
    m_networkManager->MoveShip(id, x, y);
}

void GameManager::AttackShip(uint16_t id, uint16_t targetId) {
    m_networkManager->AttackShip(id, targetId);
}

void GameManager::ShipChangeEventHandler(void *sender,
                                         admirals::events::EventArgs e) {
    const Ship *ship = static_cast<Ship *>(sender);
    switch (ship->GetAction()) {
    case ShipAction::Move: {
        MoveShip(ship->GetID(), ship->GetActionX(), ship->GetActionY());
        break;
    }
    case ShipAction::Attack: {
        break;
    }
    default: {
        break;
    }
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
