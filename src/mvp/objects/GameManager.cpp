#include "GameManager.hpp"
#include "NetworkManager.hpp"

using namespace admirals::mvp::objects;

GameManager::GameManager(const std::string &name)
    : scene::GameObject(name, 0, Vector3(0)) {
    m_networkManager = GameData::engine->MakeGameObject<NetworkManager>(
        "networkManager", (*this));
}

GameManager::~GameManager() {}

void GameManager::OnStart() {
    srand(time(NULL));

    m_debugText = GameData::engine->MakeUIElement<UI::TextElement>(
        "gameDebug", 0, "", Vector2(220, 40), Color::RED);
}

void GameManager::OnUpdate() {
    if (!m_gameStarted) {
        return;
    }

    m_debugText->SetText("Coins: " + std::to_string(m_coins));

    // Tests for actions
    if (m_turn % 10 == 5 && !m_testActionDone) {
        m_networkManager->BuyShip(ShipType::Destroyer);
        m_testActionDone = true;
    }

    if (m_turn % 10 == 0 && m_testActionDone) {
        auto it = std::find_if(
            m_ships.begin(), m_ships.end(),
            [this](const std::pair<uint16_t, std::shared_ptr<Ship>> &pair) {
                return pair.second->GetPlayerId() == m_playerId;
            });

        uint16_t id = it->second->GetId();

        m_networkManager->MoveShip(id, rand() % 10, rand() % 10);
        m_testActionDone = false;
    }
}

void GameManager::UpdateBoard(int turn, int coins, int baseHealth,
                              int enemyBaseHealth,
                              const std::map<uint16_t, ShipData> &ships) {
    m_turn = turn;
    m_coins = coins;
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
            it->second->Move(ship.x, ship.y);
            it->second->SetHealth(ship.health);
        } else {
            if (m_debug)
                printf("Creating ship %d\n", ship.id);
            std::shared_ptr<Ship> newShip =
                GameData::engine->MakeGameObject<Ship>(
                    "ship" + std::to_string(ship.id), Vector3(0, 0, 2),
                    m_cellSize, m_atlas);

            newShip->Move(ship.x, ship.y);
            newShip->SetId(ship.id);
            newShip->SetHealth(ship.health);
            newShip->SetPlayerId(ship.owner);
            newShip->SetType(ship.type);
            m_ships[ship.id] = newShip;
        }
    }

    // Remove ships in m_ships that are not in ships
    for (auto it = m_ships.begin(); it != m_ships.end();) {
        const uint16_t id = it->first;

        if (ships.find(id) == ships.end()) {
            if (m_debug)
                printf("Removing ship %d\n", it->second->GetId());
            it = m_ships.erase(it);
        } else {
            it++;
        }
    }
}
