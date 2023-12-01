#include "MvpServer.hpp"
#include <iostream>

using namespace admirals::net;
using namespace admirals::mvp;

bool MvpServer::OnClientConnect(std::shared_ptr<Connection>) { return true; }

void MvpServer::OnClientDisconnect(std::shared_ptr<Connection> client) {
    if (!m_gameStarted || m_gamePaused) {
        return;
    }

    if (--m_connectedPlayers == 0) {
        // TODO: Reset game
        // ResetGame();
    }

    if (client->GetID() == m_player1.id) {
        m_player1.ready = false;
        m_player1.connected = false;
    } else {
        m_player2.ready = false;
        m_player2.connected = false;
    }

    PauseGame();
}

void MvpServer::OnClientValidated(std::shared_ptr<Connection> client) {
    if (m_connectedPlayers >= 2) {
        client->Disconnect();
        return;
    }

    m_connectedPlayers++;

    // Set the previous owner to the first player not connected, or 0 if none
    // are not connected
    uint32_t oldOwner;
    if (!m_player1.connected && !m_player2.connected) {
        oldOwner = 0;
    } else if (!m_player1.connected) {
        oldOwner = m_player1.id;
    } else {
        oldOwner = m_player2.id;
    }

    UpdatePlayer(oldOwner, client->GetID());
}

void MvpServer::OnMessage(std::shared_ptr<Connection> client,
                          Message &message) {
    // Messages that should be processed regardless of game state
    switch (message.header.id) {
    case NetworkMessageTypes::PlayerReady: {
        PlayerReady(client);
        break;
    }

    case NetworkMessageTypes::GameStop: {
        StopGame();
        break;
    }

    default:
        break;
    }

    if (m_gamePaused) {
        return;
    }

    // Messages that should only be processed if the game is running
    switch (message.header.id) {
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

void MvpServer::ProcessTurn() {
    // Process incoming actions
    Update();

    if (m_debug) {
        std::cout << "Player 1, connected: " << m_player1.connected
                  << " ready: " << m_player1.ready << std::endl;
        std::cout << "Player 2, connected: " << m_player2.connected
                  << " ready: " << m_player2.ready << std::endl;
        std::cout << "Game, started: " << m_gameStarted
                  << " paused: " << m_gamePaused << std::endl;
    }

    if (!m_gameStarted || m_gamePaused) {
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

void MvpServer::EnterServerLoop() {
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        ProcessTurn();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000 / TICK_RATE) - duration);
    }
}

bool MvpServer::ShipAtLocation(int x, int y) { return m_board[x][y] != 0; }

bool MvpServer::ShipExists(PlayerData &player, uint16_t id) {
    return player.ships.contains(id);
}

void MvpServer::StartGame() {
    if (m_debug) {
        std::cout << "Starting game" << std::endl;
    }
    m_gameStarted = true;
    Message msg;
    msg.header.id = NetworkMessageTypes::GameStart;
    MessageAllClients(msg);
}

void MvpServer::StopGame() {
    if (m_debug) {
        std::cout << "Stopping game" << std::endl;
    }
    m_gameStarted = false;
    // TODO: Reset game
    // ResetGame();
    Message msg;
    msg.header.id = NetworkMessageTypes::GameStop;
    MessageAllClients(msg);
}

void MvpServer::PauseGame() {
    if (m_debug) {
        std::cout << "Pausing game" << std::endl;
    }
    m_gamePaused = true;
    Message msg;
    msg.header.id = NetworkMessageTypes::GamePause;
    MessageAllClients(msg);
}

void MvpServer::ResumeGame() {
    if (m_debug) {
        std::cout << "Resuming game" << std::endl;
    }
    m_gamePaused = false;
    Message msg;
    msg.header.id = NetworkMessageTypes::GameResume;
    MessageAllClients(msg);
}

void MvpServer::UpdatePlayer(uint32_t oldOwner, uint32_t newOwner) {
    PlayerData &player = oldOwner == m_player1.id ? m_player1 : m_player2;
    player.id = newOwner;
    player.connected = true;

    // 0 = no previous owner
    if (oldOwner == 0) {
        return;
    };

    for (auto &ship : player.ships) {
        ship.second.owner = newOwner;
    }
}

void MvpServer::PlayerReady(std::shared_ptr<Connection> client) {
    const uint8_t isPlayer1 = client->GetID() == m_player1.id;
    if (isPlayer1) {
        m_player1.ready = true;
    } else {
        m_player2.ready = true;
    }
    Message msg;
    msg.header.id = NetworkMessageTypes::ReadyConfirmation;
    msg << client->GetID() << isPlayer1;
    MessageClient(client, msg);

    // Should probably be moved somewhere else
    if (m_player1.ready && m_player2.ready) {
        if (m_gamePaused) {
            ResumeGame();
        } else {
            StartGame();
        }
    }
}

void MvpServer::BuyShip(std::shared_ptr<Connection> &client, Message &message) {
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
        std::cout << "Player " << clientID << " bought ship " << type << "at ("
                  << x << ", " << y << ")\n";
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

void MvpServer::MoveShip(std::shared_ptr<Connection> client, Message &message) {
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

void MvpServer::AttackShip(std::shared_ptr<Connection> client,
                           Message &message) {
    // Do not process ship actions if the game is paused
    if (m_gamePaused) {
        return;
    }
    uint16_t id, targetID;

    message >> targetID >> id;

    // Ship ID 0 is invalid
    if (id == 0 || targetID == 0) {
        return;
    }

    int clientID = client->GetID();
    PlayerData &player = clientID == m_player1.id ? m_player1 : m_player2;
    PlayerData &targetPlayer = clientID == m_player1.id ? m_player2 : m_player1;

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

void MvpServer::ProcessShips() {
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

void MvpServer::BroadcastState() {
    Message msg;
    msg.header.id = NetworkMessageTypes::BoardUpdate;
    msg << m_turn << m_player1.coins << m_player2.coins << m_player1.baseHealth
        << m_player2.baseHealth;

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
        std::cout << "Player 1 ships: " << static_cast<int>(m_player1.numShips)
                  << " Player 2 ships: " << static_cast<int>(m_player2.numShips)
                  << std::endl;
        std::cout << "Player 1 base health: " << m_player1.baseHealth
                  << " Player 2 base health: " << m_player2.baseHealth
                  << std::endl;
    }

    MessageAllClients(msg);
}
