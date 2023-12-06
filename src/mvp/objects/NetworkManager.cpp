#include "objects/NetworkManager.hpp"
#include "MvpServer.hpp"
#include "commontypes.hpp"

using namespace admirals::mvp::objects;
using namespace admirals::net;

NetworkManager::NetworkManager(const std::string &name,
                               GameManager &gameManager)
    : GameObject(name), m_gameManager(gameManager) {}

NetworkManager::~NetworkManager() {
    if (m_isHost) {
        m_serverThread.join();
    }
}

void NetworkManager::OnStart(const EngineContext &) {}

void NetworkManager::OnUpdate(const EngineContext &) { HandleMessages(); }

bool NetworkManager::StartAndConnectToServer(uint16_t port,
                                             const size_t maxTries) {
    m_isHost = true;

    m_serverThread = std::thread([this, port]() {
        MvpServer server(port);
        server.Start();
        server.EnterServerLoop();
    });

    return ConnectToServer("127.0.0.1", port, maxTries);
}

bool NetworkManager::ConnectToServer(const std::string &ip, uint16_t port,
                                     const size_t maxTries) {
    for (size_t i = 0; i < maxTries; i++) {
        if (m_debug) {
            printf("Trying to connect to the server...\n");
        }

        Connect(ip, std::to_string(port));
        if (IsConnected()) {
            if (m_debug) {
                printf("Connected to the server\n");
            }

            // Should probably be called later by the user conciously and not
            // here
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    Send(msg);
}

void NetworkManager::HandleMessages() {
    if (!IsConnected()) {
        if (m_gameManager.GameStarted())
            m_gameManager.AbortGame();
        return;
    }

    const size_t size = Incoming().Size();
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
            GameStop(msg);
            break;
        }

        case NetworkMessageTypes::GamePause: {
            GamePause();
            break;
        }

        case NetworkMessageTypes::GameResume: {
            GameResume();
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
    uint8_t isTopPlayer;
    msg >> isTopPlayer >> playerId;

    m_playerId = playerId;
    m_gameManager.SetPlayerId(playerId, static_cast<bool>(isTopPlayer));

    if (m_debug) {
        printf("ReadyConfirmation: %d, Is top player: %d\n", playerId,
               isTopPlayer);
    }
}

void NetworkManager::GameStart() {
    if (m_debug)
        printf("StartGame\n");
    m_gameManager.StartGame();
}

void NetworkManager::GameStop(Message &msg) {
    uint8_t winner;
    msg >> winner;
    if (m_debug)
        printf("StopGame\n");
    m_gameManager.StopGame(winner);
}

void NetworkManager::GamePause() {
    if (m_debug)
        printf("PauseGame\n");
    m_gameManager.PauseGame();
}

void NetworkManager::GameResume() {
    if (m_debug)
        printf("ResumeGame\n");
    m_gameManager.ResumeGame();
}

void NetworkManager::UpdateBoard(Message &msg) {
    uint8_t playerTopShips;
    uint8_t playerBottomShips;
    msg >> playerBottomShips >> playerTopShips;

    std::map<uint16_t, ShipData> ships;
    for (int i = 0; i < playerTopShips + playerBottomShips; i++) {
        ShipData ship;
        msg >> ship;
        ships[ship.id] = ship;
    }

    uint16_t turn;
    uint16_t playerTopCoins;
    uint16_t playerBottomCoins;
    msg >> playerBottomCoins >> playerTopCoins >> turn;

    const bool isTopPlayer = m_gameManager.GetIsTopPlayer();
    const int player_coins = isTopPlayer ? playerTopCoins : playerBottomCoins;

    if (m_debug) {
        printf("Turn: %d\n", turn);
        printf("Player 1 coins: %d\tPlayer 2 coins: %d\n", playerTopCoins,
               playerBottomCoins);
        printf("Player 1 ships: %d\tPlayer 2 ships: %d\n", playerTopShips,
               playerBottomShips);
    }

    m_gameManager.UpdateBoard(turn, player_coins, ships);
}