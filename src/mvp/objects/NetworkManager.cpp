#include "NetworkManager.hpp"
#include "GameManager.hpp"
#include "commontypes.hpp"

using namespace admirals::mvp::objects;
using namespace admirals::net;

NetworkManager::NetworkManager(const std::string &name,
                               GameManager &gameManager)
    : GameObject(name, 0, Vector3(0)), m_gameManager(gameManager) {}

NetworkManager::~NetworkManager() {}

void NetworkManager::OnStart(const EngineContext &ctx) {}

void NetworkManager::OnUpdate(const EngineContext &ctx) { HandleMessages(); }

bool NetworkManager::ConnectToServer(std::string ip, std::string port,
                                     const size_t maxTries) {
    for (size_t i = 0; i < maxTries; i++) {
        if (m_debug) {
            printf("Trying to connect to the server...\n");
        }

        Connect(ip, port);
        if (IsConnected()) {
            if (m_debug) {
                printf("Connected to the server\n");
            }

            // Should probably be called later by the user conciously and not here
            ReadyUp();
            
            return true;
        }
    }

    if (m_debug) {
        printf("Failed to connect to the server\n");
    }

    return false;
}

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
    }

    Message msg;
    msg.header.id = NetworkMessageTypes::PlayerReady;
    printf("Sending ready message\n");
    Send(msg);
}

void NetworkManager::HandleMessages() {
    const size_t size = Incoming().Size();
    for (size_t i = 0; i < size; i++) {
        auto msg = Incoming().Front().message;
        Incoming().PopFront();

        printf("Message id: %d\n", msg.header.id);
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
        default:
            break;
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
    uint8_t player1Ships;
    uint8_t player2Ships;
    msg >> player1Ships >> player2Ships;

    std::map<uint16_t, ShipData> ships;
    for (int i = 0; i < player1Ships + player2Ships; i++) {
        ShipData ship;
        msg >> ship;
        ships[ship.id] = ship;
    }

    uint16_t turn;
    uint16_t player1Coins;
    uint16_t player2Coins;
    uint16_t player1BaseHealth;
    uint16_t player2BaseHealth;
    msg >> player2BaseHealth >> player1BaseHealth >> player2Coins >>
        player1Coins >> turn;

    const int player_coins = m_playerId % 2 == 0 ? player1Coins : player2Coins;

    const int base_health =
        m_playerId % 2 == 0 ? player1BaseHealth : player2BaseHealth;
    const int enemy_base_health =
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