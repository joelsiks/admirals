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

    if (client->GetID() == m_playerTop.id) {
        m_playerTop.ready = false;
        m_playerTop.connected = false;
    } else {
        m_playerBottom.ready = false;
        m_playerBottom.connected = false;
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
    if (!m_playerTop.connected && !m_playerBottom.connected) {
        oldOwner = 0;
    } else if (!m_playerTop.connected) {
        oldOwner = m_playerTop.id;
    } else {
        oldOwner = m_playerBottom.id;
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
        std::cout << "Player 1, connected: " << m_playerTop.connected
                  << " ready: " << m_playerTop.ready << std::endl;
        std::cout << "Player 2, connected: " << m_playerBottom.connected
                  << " ready: " << m_playerBottom.ready << std::endl;
        std::cout << "Game, started: " << m_gameStarted
                  << " paused: " << m_gamePaused << std::endl;
    }

    if (!m_gameStarted || m_gamePaused) {
        return;
    }

    m_turn++;

    // Give coins every second
    if (m_turn % TICK_RATE == 0) {
        m_playerTop.coins += 2;
        m_playerBottom.coins += 2;
    }

    // Give coins from tresure islands
    ProcessGoldGeneration(m_playerTop);
    ProcessGoldGeneration(m_playerBottom);

    // Process ship actions
    ProcessShips(m_playerTop.ships);
    ProcessShips(m_playerBottom.ships);

    // Remove ships with 0 health
    ProcessDeadShips(m_playerTop.ships);
    ProcessDeadShips(m_playerBottom.ships);

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
    PlayerData &player =
        oldOwner == m_playerTop.id ? m_playerTop : m_playerBottom;
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
    const uint8_t isTopPlayer = client->GetID() == m_playerTop.id;
    if (isTopPlayer) {
        m_playerTop.ready = true;
    } else {
        m_playerBottom.ready = true;
    }
    Message msg;
    msg.header.id = NetworkMessageTypes::ReadyConfirmation;
    msg << client->GetID() << isTopPlayer;
    MessageClient(client, msg);

    // Should probably be moved somewhere else
    if (m_playerTop.ready && m_playerBottom.ready) {
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
    PlayerData &player =
        clientID == m_playerTop.id ? m_playerTop : m_playerBottom;
    if (player.coins < ShipInfoMap[type].Cost) {
        return;
    }

    // Assumed that the ship is spawned besides the base
    // Could do other logic instead
    // Ship spawn location is (1, 0) or (-2, -1)
    const int x = clientID == m_playerTop.id ? 1 : BOARD_SIZE - 2;
    const int y = clientID == m_playerTop.id ? 0 : BOARD_SIZE - 1;

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
    PlayerData &player =
        clientID == m_playerTop.id ? m_playerTop : m_playerBottom;

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
    PlayerData &player =
        clientID == m_playerTop.id ? m_playerTop : m_playerBottom;
    PlayerData &targetPlayer =
        clientID == m_playerTop.id ? m_playerBottom : m_playerTop;

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

void MvpServer::CheckTreasureIsland(int treasure_x, int treasure_y,
                                    PlayerData &player) {
    std::map<uint16_t, admirals::mvp::ShipData> &shipList = player.ships;
    for (int y = treasure_y - 1; y <= treasure_y + 1; y++) {
        if (y < 0 || y >= BOARD_SIZE) {
            continue;
        }
        for (int x = treasure_x - 1; x <= treasure_x + 1; x++) {
            if (x < 0 || x >= BOARD_SIZE) {
                continue;
            }
            const uint16_t shipId = m_board[x][y];
            if (shipId != 0) {
                if (auto it = shipList.find(shipId); it != shipList.end()) {
                    player.coins += 5;
                }
            }
        }
    }
}

void MvpServer::ProcessGoldGeneration(PlayerData &player) {
    CheckTreasureIsland(2, 7, player);
    CheckTreasureIsland(7, 2, player);
}

void MvpServer::DamageNearbyEnemies(admirals::mvp::ShipData &ship) {
    const bool isTopPlayer = ship.owner == m_playerTop.id;
    std::map<uint16_t, admirals::mvp::ShipData> &enemyShips =
        isTopPlayer ? m_playerBottom.ships : m_playerTop.ships;
    for (int y = ship.y - 1; y <= ship.y + 1; y++) {
        if (y < 0 || y >= BOARD_SIZE) {
            continue;
        }

        for (int x = ship.x - 1; x <= ship.x + 1; x++) {
            if (x < 0 || x >= BOARD_SIZE) {
                continue;
            }

            // If bottom player, damage top player base if nearby
            if (!isTopPlayer && x == 0 && y == 1) {
                if (m_playerTop.baseHealth < ShipInfoMap[ship.type].Damage) {
                    m_playerTop.baseHealth = 0;
                } else {
                    m_playerTop.baseHealth -= ShipInfoMap[ship.type].Damage;
                }
            }

            // If top player, damage bottom player base if nearby
            if (isTopPlayer && x == BOARD_SIZE - 1 && y == BOARD_SIZE - 2) {
                if (m_playerBottom.baseHealth < ShipInfoMap[ship.type].Damage) {
                    m_playerBottom.baseHealth = 0;
                } else {
                    m_playerBottom.baseHealth -= ShipInfoMap[ship.type].Damage;
                }
            }

            const uint16_t shipId = m_board[x][y];
            if (auto it = enemyShips.find(shipId); it != enemyShips.end()) {
                auto *enemy = &it->second;
                if (enemy->health < ShipInfoMap[ship.type].Damage) {
                    enemy->health = 0;
                } else {
                    enemy->health -= ShipInfoMap[ship.type].Damage;
                }
            }
        }
    }
}

void MvpServer::ProcessShips(
    std::map<uint16_t, admirals::mvp::ShipData> &ships) {
    // TODO: Process ship actions correctly
    for (auto &ship : ships) {
        if (ship.second.action == ShipAction::Move &&
            ship.second.moveData.actionY == ship.second.y &&
            ship.second.moveData.actionX == ship.second.x) {
            ship.second.action = ShipAction::None;
            continue;
        }
        switch (ship.second.action) {
        case ShipAction::None:
            DamageNearbyEnemies(ship.second);
            break;
        case ShipAction::Attack:
            break;
        case ShipAction::Move:
            if (m_board[ship.second.moveData.actionX]
                       [ship.second.moveData.actionY] != 0) {
                continue;
            }
            m_board[ship.second.x][ship.second.y] = 0;
            ship.second.x = ship.second.moveData.actionX;
            ship.second.y = ship.second.moveData.actionY;
            m_board[ship.second.x][ship.second.y] = ship.second.id;
            break;
        default:
            break;
        }
    }
}

void MvpServer::ProcessDeadShips(
    std::map<uint16_t, admirals::mvp::ShipData> &ships) {
    for (auto it = ships.begin(); it != ships.end();) {
        const auto ship = it->second;
        if (ship.health == 0) {
            const auto owner = ship.owner;
            (owner == m_playerTop.id ? m_playerTop : m_playerBottom).numShips--;
            m_board[ship.x][ship.y] = 0;
            it = ships.erase(it);
        } else {
            it++;
        }
    }
}

void MvpServer::BroadcastState() {
    Message msg;
    msg.header.id = NetworkMessageTypes::BoardUpdate;
    msg << m_turn << m_playerTop.coins << m_playerBottom.coins
        << m_playerTop.baseHealth << m_playerBottom.baseHealth;

    for (auto &ship : m_playerTop.ships) {
        msg << ship.second;
    }
    for (auto &ship : m_playerBottom.ships) {
        msg << ship.second;
    }

    msg << m_playerTop.numShips << m_playerBottom.numShips;

    if (m_debug) {
        std::cout << "Turn: " << m_turn << std::endl;
        std::cout << "Player 1 coins: " << m_playerTop.coins
                  << " Player 2 coins: " << m_playerBottom.coins << std::endl;
        std::cout << "Player 1 ships: "
                  << static_cast<int>(m_playerTop.numShips)
                  << " Player 2 ships: "
                  << static_cast<int>(m_playerBottom.numShips) << std::endl;
        std::cout << "Player 1 base health: " << m_playerTop.baseHealth
                  << " Player 2 base health: " << m_playerBottom.baseHealth
                  << std::endl;
    }

    MessageAllClients(msg);
}
