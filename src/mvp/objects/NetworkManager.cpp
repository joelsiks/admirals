#include "NetworkManager.hpp"
#include "GameManager.hpp"
#include "commontypes.hpp"

using namespace admirals::mvp::objects;
using namespace admirals::net;

NetworkManager::NetworkManager(const std::string &name,
                               GameManager &gameManager)
    : GameObject(name, 0, Vector3(0)), m_gameManager(gameManager) {}

NetworkManager::~NetworkManager() {}

void NetworkManager::OnStart() {
    printf("NetworkManager::OnStart()\n");
    m_debugText = GameData::engine->MakeUIElement<UI::TextElement>(
        "networkDebug", 0, "", Vector2(220, 40), Color::RED);
    m_debugText->SetDisplayPosition(UI::DisplayPosition::UpperRight);

    // Should probably be called later and not here
    Connect("127.0.0.1", "60000");
    while (!IsConnected()) {
        printf("Waiting for connection...\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    printf("Connected!\n");
    if (m_debug)
        m_debugText->SetText("Connected!");

    // Should probably be called later and not here
    ReadyUp();
}

void NetworkManager::OnUpdate() { HandleMessages(); }

void NetworkManager::BuyShip(uint8_t type) {
    if (m_debug)
        printf("NetworkManager::BuyShip()\n");

    Message msg;
    msg.header.id = NetworkMessageTypes::BuyShip;
    msg << type;
    Send(msg);
}

void NetworkManager::MoveShip(uint16_t id, uint8_t x, uint8_t y) {
    if (m_debug)
        printf("NetworkManager::MoveShip()\n");

    Message msg;
    msg.header.id = NetworkMessageTypes::MoveShip;
    msg << id << x << y;
    Send(msg);
}

void NetworkManager::AttackShip(uint16_t id, uint16_t targetId) {
    if (m_debug)
        printf("NetworkManager::AttackShip()\n");

    Message msg;
    msg.header.id = NetworkMessageTypes::AttackShip;
    msg << id << targetId;
    Send(msg);
}

void NetworkManager::ReadyUp() {
    if (m_debug) {
        printf("NetworkManager::ReadyUp()\n");
        m_debugText->SetText("ReadyUp!");
    }

    Message msg;
    msg.header.id = NetworkMessageTypes::PlayerReady;
    Send(msg);
}

void NetworkManager::HandleMessages() {
    size_t size = Incoming().Size();
    for (size_t i = 0; i < size; i++) {
        auto msg = Incoming().Front().message;
        Incoming().PopFront();

        switch (msg.header.id) {
        case NetworkMessageTypes::ReadyConfirmation: {
            ReadyUpResponse(msg);
            break;
        }

        case NetworkMessageTypes::GameStart: {
            GameStart();
            break;
        }

        case NetworkMessageTypes::GameStop: {
            GameStop();
            break;
        }

        case NetworkMessageTypes::BoardUpdate: {
            UpdateBoard(msg);
            break;
        }
        }
    }
}

void NetworkManager::ReadyUpResponse(Message &msg) {
    uint32_t playerId;
    msg >> playerId;

    m_playerId = playerId;
    m_gameManager.SetPlayerId(playerId);

    if (m_debug) {
        printf("ReadyConfirmation: %d\n", playerId);
        m_debugText->SetText("ID: " + std::to_string(playerId));
    }
}

void NetworkManager::GameStart() {
    if (m_debug)
        printf("StartGame\n");
    m_gameManager.StartGame();
}

void NetworkManager::GameStop() {
    if (m_debug)
        printf("StopGame\n");
    m_gameManager.StopGame();
}

void NetworkManager::UpdateBoard(Message &msg) {
    uint8_t player1Ships, player2Ships;
    msg >> player1Ships >> player2Ships;

    std::vector<ShipData> ships;
    for (int i = 0; i < player1Ships + player2Ships; i++) {
        ShipData ship;
        msg >> ship;
        ships.push_back(ship);
    }

    uint16_t turn, player1Coins, player2Coins, player1BaseHealth,
        player2BaseHealth;

    msg >> player2BaseHealth >> player1BaseHealth >> player2Coins >>
        player1Coins >> turn;

    int player_coins = m_playerId % 2 == 0 ? player1Coins : player2Coins;

    int base_health =
        m_playerId % 2 == 0 ? player1BaseHealth : player2BaseHealth;
    int enemy_base_health =
        m_playerId % 2 == 0 ? player2BaseHealth : player1BaseHealth;

    if (m_debug) {
        printf("Turn: %d\n", turn);
        printf("Player 1 coins: %d\tPlayer 2 coins: %d\n", player1Coins,
               player2Coins);
        printf("Player 1 ships: %d\tPlayer 2 ships: %d\n", player1Ships,
               player2Ships);
        printf("Player 1 base health: %d\tPlayer 2 base health: %d\n",
               player1BaseHealth, player2BaseHealth);
    }

    m_gameManager.UpdateBoard(turn, player_coins, base_health,
                              enemy_base_health, ships);
}