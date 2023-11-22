#include <iostream>

#include "Server.hpp"
#include "commontypes.hpp"

// Number of ticks per second
#define TICK_RATE 1

#define BOARD_SIZE 10

using namespace admirals::net;
using namespace admirals::mvp;

struct PlayerData {
    uint16_t coins = 0;
    uint16_t baseHealth = 0;
    uint32_t id = 0;
    uint8_t numShips = 0;
    std::map<uint16_t, ShipData> ships = {};
};

class MvpServer : public Server {

public:
    MvpServer(uint16_t port) : Server(port) { m_turn = 0; }

    bool OnClientConnect(std::shared_ptr<Connection>) override { return true; }

    void OnClientDisconnect(std::shared_ptr<Connection>) override {
        m_connectedPlayers--;
        if (m_gameStarted) {
            StopGame();
        }
    }

    void OnClientValidated(std::shared_ptr<Connection> client) override {
        if (m_connectedPlayers >= 2) {
            client->Disconnect();
            return;
        }

        if (m_connectedPlayers++ == 0) {
            m_player1.id = client->GetID();

        } else {
            m_player2.id = client->GetID();
        }
    }

    void OnMessage(std::shared_ptr<Connection> client,
                   Message &message) override {
        switch (message.header.id) {
        case NetworkMessageTypes::PlayerReady: {
            PlayerReady(client);
            break;
        }

        case NetworkMessageTypes::GameStop: {
            StopGame();
            break;
        }

        case NetworkMessageTypes::BuyShip: {
            BuyShip(client, message);
            break;
        }

        case NetworkMessageTypes::MoveShip: {
            MoveShip(client, message);
            break;
        }

        case NetworkMessageTypes::AttackShip: {
            AttackShip(client, message);
            break;
        }
        default:
            break;
        }
    }

    void ProcessTurn() {

        // Process incoming actions
        Update();

        if (!m_gameStarted) {
            return;
        }

        m_turn++;

        // Give coins every second
        if (m_turn % TICK_RATE == 0) {
            m_player1.coins += 10;
            m_player2.coins += 10;
        }

        // Process ship actions
        ProcessShips();

        // Broadcast state
        BroadcastState();
    }

private:
    bool ShipAtLocation(int x, int y) { return m_board[x][y] != 0; }

    bool ShipExists(PlayerData &player, uint16_t id) {
        return player.ships.contains(id);
    }

    void StartGame() {
        m_gameStarted = true;
        Message msg;
        msg.header.id = NetworkMessageTypes::GameStart;
        MessageAllClients(msg);
    }

    void StopGame() {
        std::cout << "Stopping game" << std::endl;
        m_gameStarted = false;
        // TODO: Reset game
        // ResetGame();
        Message msg;
        msg.header.id = NetworkMessageTypes::GameStop;
        MessageAllClients(msg);
    }

    void PlayerReady(std::shared_ptr<Connection> client) {
        if (client->GetID() == m_player1.id) {
            m_player1Ready = true;
        } else {
            m_player2Ready = true;
        }
        Message msg;
        msg.header.id = NetworkMessageTypes::ReadyConfirmation;
        msg << client->GetID();
        MessageClient(client, msg);

        // Should probably be moved somewhere else
        if (m_player1Ready && m_player2Ready) {
            StartGame();
        }
    }

    void BuyShip(std::shared_ptr<Connection> &client, Message &message) {
        uint8_t type;
        message >> type;

        const uint32_t clientID = client->GetID();
        PlayerData &player = clientID == m_player1.id ? m_player1 : m_player2;
        if (player.coins < ShipInfoMap[type].Cost) {
            return;
        }

        // Assumed that the ship is spawned besides the base
        // Could do other logic instead
        // Ship spawn location is (1, 0) or (-2, -1)
        const int x = clientID == m_player1.id ? 1 : -1 + BOARD_SIZE - 1;
        const int y = clientID == m_player1.id ? 0 : BOARD_SIZE - 1;

        if (m_debug) {
            std::cout << "Player " << clientID << " bought ship " << type
                      << "at (" << x << ", " << y << ")\n";
        }

        // Check if there is a ship in the way
        if (ShipAtLocation(x, y)) {
            return;
        }

        const ShipData ship(m_shipID++, type, x, y, ShipInfoMap[type].Health,
                            clientID);

        player.coins -= ShipInfoMap[type].Cost;
        player.ships[ship.id] = ship;
        player.numShips++;
        m_board[x][y] = ship.id;
    }

    void MoveShip(std::shared_ptr<Connection> client, Message &message) {
        uint16_t id;
        uint8_t x;
        uint8_t y;
        message >> y >> x >> id;

        // Ship ID 0 is invalid
        if (id <= 0) {
            return;
        }

        // Must be within the board
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            return;
        }

        // Check if a ship already exists at the target location
        if (ShipAtLocation(x, y)) {
            return;
        }

        if (m_debug) {
            std::cout << "Player " << client->GetID() << " moved ship " << id
                      << " to (" << static_cast<int>(x) << ", "
                      << static_cast<int>(y) << ")" << std::endl;
        }

        const int clientID = client->GetID();
        PlayerData &player = clientID == m_player1.id ? m_player1 : m_player2;

        // Check if the ship exists
        if (!ShipExists(player, id)) {
            return;
        }

        ShipData &ship = player.ships[id];
        ship.action = ShipAction::Move;
        ship.moveData.actionX = x;
        ship.moveData.actionY = y;
    }

    void AttackShip(std::shared_ptr<Connection> client, Message &message) {
        uint16_t id, targetID;

        message >> targetID >> id;

        // Ship ID 0 is invalid
        if (id == 0 || targetID == 0) {
            return;
        }

        int clientID = client->GetID();
        PlayerData &player = clientID == m_player1.id ? m_player1 : m_player2;
        PlayerData &targetPlayer =
            clientID == m_player1.id ? m_player2 : m_player1;

        // Check if the ship exists
        if (!ShipExists(player, id)) {
            return;
        }

        // Check if the target ship exists
        if (!ShipExists(targetPlayer, targetID)) {
            return;
        }

        ShipData &ship = player.ships[id];
        ship.action = ShipAction::Attack;
        ship.attackTargetID = targetID;
    }

    void ProcessShips() {
        // TODO: Process ship actions correctly
        for (auto &ship : m_player1.ships) {
            if (ship.second.action == ShipAction::Move) {
                if (m_board[ship.second.moveData.actionX]
                           [ship.second.moveData.actionY] != 0) {
                    continue;
                }
                m_board[ship.second.x][ship.second.y] = 0;
                ship.second.x = ship.second.moveData.actionX;
                ship.second.y = ship.second.moveData.actionY;
                m_board[ship.second.x][ship.second.y] = ship.second.id;
            }
            ship.second.action = ShipAction::None;
        }
        for (auto &ship : m_player2.ships) {
            if (ship.second.action == ShipAction::Move) {
                if (m_board[ship.second.moveData.actionX]
                           [ship.second.moveData.actionY] != 0) {
                    continue;
                }
                m_board[ship.second.x][ship.second.y] = 0;
                ship.second.x = ship.second.moveData.actionX;
                ship.second.y = ship.second.moveData.actionY;
                m_board[ship.second.x][ship.second.y] = ship.second.id;
            }
            ship.second.action = ShipAction::None;
        }
    }

    void BroadcastState() {
        Message msg;
        msg.header.id = NetworkMessageTypes::BoardUpdate;
        msg << m_turn << m_player1.coins << m_player2.coins
            << m_player1.baseHealth << m_player2.baseHealth;

        for (auto &ship : m_player1.ships) {
            msg << ship.second;
        }
        for (auto &ship : m_player2.ships) {
            msg << ship.second;
        }

        msg << m_player1.numShips << m_player2.numShips;

        if (m_debug) {
            std::cout << "Turn: " << m_turn << std::endl;
            std::cout << "Player 1 coins: " << m_player1.coins
                      << " Player 2 coins: " << m_player2.coins << std::endl;
            std::cout << "Player 1 ships: "
                      << static_cast<int>(m_player1.numShips)
                      << " Player 2 ships: "
                      << static_cast<int>(m_player2.numShips) << std::endl;
            std::cout << "Player 1 base health: " << m_player1.baseHealth
                      << " Player 2 base health: " << m_player2.baseHealth
                      << std::endl;
        }

        MessageAllClients(msg);
    }

private:
    uint16_t m_turn = 0;
    PlayerData m_player1;
    PlayerData m_player2;

    // 2D array of ship ids
    uint16_t m_board[BOARD_SIZE][BOARD_SIZE] = {};

    int m_connectedPlayers = 0;
    bool m_gameStarted = false;
    bool m_player1Ready = false;
    bool m_player2Ready = false;

    int m_shipID = 1;

    bool m_debug = false;
};

int main() {
    MvpServer server(60000);
    server.Start();

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        server.ProcessTurn();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000 / TICK_RATE) - duration);
    }
}
