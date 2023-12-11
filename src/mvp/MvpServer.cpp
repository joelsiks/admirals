#include "MvpServer.hpp"
#include <iostream>

using namespace admirals::net;
using namespace admirals::mvp;

bool MvpServer::OnClientConnect(const std::shared_ptr<Connection> &) {
    return true;
}

void MvpServer::OnClientDisconnect(std::shared_ptr<Connection> client) {
    printf("Client %d disconnected\n", client->GetID());

    if (client->GetID() == m_playerTop.id) {
        m_playerTop.ready = false;
        m_playerTop.connected = false;
        m_playerTop.id = 0;
        m_connectedPlayers--;
    } else if (client->GetID() == m_playerBottom.id) {
        m_playerBottom.ready = false;
        m_playerBottom.connected = false;
        m_playerBottom.id = 0;
        m_connectedPlayers--;
    }

    if (m_connectedPlayers == 0 && m_gameStarted) {
        ResetState();
        return;
    }

    if (m_gameStarted && !m_gamePaused)
        PauseGame();
}

void MvpServer::OnClientValidated(const std::shared_ptr<Connection> &client) {
    if (m_connectedPlayers >= 2) {
        client->Disconnect();
        return;
    }

    m_connectedPlayers++;

    // Set the previous owner to the first player not connected, or 0 if none
    // are connected
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

void MvpServer::OnMessage(const std::shared_ptr<Connection> &client,
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
    ClearDisconnectedClients();

    if (m_debug) {
        std::cout << "Player 1, connected: " << m_playerTop.connected
                  << " ready: " << m_playerTop.ready << "\n";
        std::cout << "Player 2, connected: " << m_playerBottom.connected
                  << " ready: " << m_playerBottom.ready << "\n";
        std::cout << "Game, started: " << m_gameStarted
                  << " paused: " << m_gamePaused << "\n";
    }

    if (!m_gameStarted || m_gamePaused) {
        return;
    }

    m_turn++;

    // Give coins every second
    if (m_turn % TICK_RATE == 0) {
        m_playerTop.coins += PASSIVE_INCOME;
        m_playerBottom.coins += PASSIVE_INCOME;

        // Give coins from treasure islands
        ProcessGoldGeneration();
    }

    // Process ship actions
    ProcessShips(m_playerTop.ships);
    ProcessShips(m_playerBottom.ships);
    ProcessShipsLate(m_playerTop.ships);
    ProcessShipsLate(m_playerBottom.ships);

    // Check if player has won
    if (ProcessWinCondition())
        return;

    // Remove ships with 0 health
    ProcessDeadShips(m_playerTop.ships);
    ProcessDeadShips(m_playerBottom.ships);

    // Broadcast state
    BroadcastState();
}

void MvpServer::EnterServerLoop(bool &stopServer) {
    while (!stopServer) {
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
        std::cout << "Starting game\n";
    }
    const auto baseData = ShipInfoMap[ShipType::Base];

    // Add top player base
    const ShipData baseTop =
        ShipData(m_shipID++, ShipType::Base, baseData.TopSpawns[0].x,
                 baseData.TopSpawns[0].y, baseData.Health, m_playerTop.id);
    m_playerTop.baseId = baseTop.id;
    m_playerTop.ships[baseTop.id] = baseTop;
    m_playerTop.numShips++;
    m_board[baseData.TopSpawns[0].x][baseData.TopSpawns[0].y] = baseTop.id;

    // Add bottom player base
    const ShipData baseBottom = ShipData(
        m_shipID++, ShipType::Base, baseData.BottomSpawns[0].x,
        baseData.BottomSpawns[0].y, baseData.Health, m_playerBottom.id);
    m_playerBottom.baseId = baseBottom.id;
    m_playerBottom.ships[baseBottom.id] = baseBottom;
    m_playerBottom.numShips++;
    m_board[baseData.BottomSpawns[0].x][baseData.BottomSpawns[0].y] =
        baseBottom.id;

    m_gameStarted = true;
    Message msg;
    msg.header.id = NetworkMessageTypes::GameStart;
    MessageAllClients(msg);
}

void MvpServer::StopGame(uint8_t winner) {
    if (m_debug) {
        std::cout << "Stopping game"
                  << "\n";
    }
    Message msg;
    msg.header.id = NetworkMessageTypes::GameStop;
    msg << winner;
    MessageAllClients(msg);

    ResetState();
}

void MvpServer::PauseGame() {
    if (m_debug) {
        std::cout << "Pausing game\n";
    }
    m_gamePaused = true;
    Message msg;
    msg.header.id = NetworkMessageTypes::GamePause;
    MessageAllClients(msg);
}

void MvpServer::ResumeGame() {
    if (m_debug) {
        std::cout << "Resuming game\n";
    }
    m_gamePaused = false;
    Message msg;
    msg.header.id = NetworkMessageTypes::GameResume;
    MessageAllClients(msg);
}

void MvpServer::ResetState() {
    if (m_debug) {
        std::cout << "Resetting game" << std::endl;
    }
    m_turn = 0;
    m_playerTop.coins = STARTING_COINS;
    m_playerBottom.coins = STARTING_COINS;
    m_playerTop.baseHealth = BaseMaxHealth;
    m_playerBottom.baseHealth = BaseMaxHealth;
    m_playerTop.ships.clear();
    m_playerBottom.ships.clear();
    m_playerTop.numShips = 0;
    m_playerBottom.numShips = 0;
    m_playerTop.ready = false;
    m_playerBottom.ready = false;
    m_gameStarted = false;
    m_gamePaused = false;
    m_shipID = 1;
    memset(m_board, 0, sizeof(m_board));
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

void MvpServer::PlayerReady(const std::shared_ptr<Connection> &client) {
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

void MvpServer::BuyShip(const std::shared_ptr<Connection> &client,
                        Message &message) {
    uint8_t type;
    message >> type;

    const uint32_t clientID = client->GetID();
    const bool isTopPlayer = clientID == m_playerTop.id;
    PlayerData &player = isTopPlayer ? m_playerTop : m_playerBottom;
    if (player.coins < ShipInfoMap[type].Cost) {
        return;
    }

    // Check if there is a ship in the way
    bool hasValidSpawnLocation = false;
    Location spawnLocation;
    const auto *spawns = isTopPlayer ? ShipInfoMap[type].TopSpawns
                                     : ShipInfoMap[type].BottomSpawns;
    for (int i = 0; i < ShipInfoMap[type].NumberOfSpawnLocations; i++) {
        if (!ShipAtLocation(spawns[i].x, spawns[i].y)) {
            hasValidSpawnLocation = true;
            spawnLocation = {spawns[i].x, spawns[i].y};
            break;
        }
    }

    if (m_debug) {
        std::cout << "Player " << clientID << " bought ship " << type << "at ("
                  << spawnLocation.x << ", " << spawnLocation.y << ")\n";
    }

    if (hasValidSpawnLocation) {
        const ShipData ship(m_shipID++, type, spawnLocation.x, spawnLocation.y,
                            ShipInfoMap[type].Health, clientID);

        player.coins -= ShipInfoMap[type].Cost;
        player.ships[ship.id] = ship;
        player.numShips++;
        m_board[spawnLocation.x][spawnLocation.y] = ship.id;
    }
}

void MvpServer::MoveShip(const std::shared_ptr<Connection> &client,
                         Message &message) {
    uint16_t id;
    uint8_t x;
    uint8_t y;
    message >> y >> x >> id;

    // Ship ID 0 is invalid
    if (id <= 0) {
        return;
    }

    // Must be within the board
    if (x < 0 || x >= BoardSize || y < 0 || y >= BoardSize) {
        return;
    }

    // Check if a ship already exists at the target location
    if (ShipAtLocation(x, y)) {
        return;
    }

    if (m_debug) {
        std::cout << "Player " << client->GetID() << " moved ship " << id
                  << " to (" << static_cast<int>(x) << ", "
                  << static_cast<int>(y) << ")\n";
    }

    const auto clientID = client->GetID();
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

void MvpServer::AttackShip(const std::shared_ptr<Connection> &client,
                           Message &message) {
    // Do not process ship actions if the game is paused
    if (m_gamePaused) {
        return;
    }
    uint16_t id;
    uint16_t targetID;
    message >> targetID >> id;

    // Ship ID 0 is invalid
    if (id == 0 || targetID == 0) {
        return;
    }

    const auto clientID = client->GetID();
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

void MvpServer::IncrementGoldByShipId(uint16_t shipId) {
    if (m_playerTop.ships.contains(shipId) &&
        m_playerTop.ships[shipId].action == ShipAction::None) {
        m_playerTop.coins += ISLAND_INCOME;
    } else if (m_playerBottom.ships.contains(shipId) &&
               m_playerBottom.ships[shipId].action == ShipAction::None) {
        m_playerBottom.coins += ISLAND_INCOME;
    } else {
        // Something went wrong here
    }
}

static bool coordinateInBounds(int x, int y) {
    return x >= 0 && x < BoardSize && y >= 0 && y < BoardSize;
}

void MvpServer::CheckTreasureIsland(int tx, int ty) {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (!coordinateInBounds(tx + dx, ty + dy)) {
                continue;
            }

            const uint16_t id = m_board[tx + dx][ty + dy];
            // find invalid shipIds
            if (id == 0) {
                continue;
            }

            IncrementGoldByShipId(m_board[tx + dx][ty + dy]);
        }
    }
}

void MvpServer::ProcessGoldGeneration() {
    for (const auto &loc : IslandLocations) {
        CheckTreasureIsland(loc.x, loc.y);
    }
}

void MvpServer::DamageNearbyEnemies(ShipData &ship) {
    const bool isTopPlayer = ship.owner == m_playerTop.id;
    auto &enemy = isTopPlayer ? m_playerBottom : m_playerTop;
    auto &enemyShips = enemy.ships;

    for (int y = ship.location.y - 1; y <= ship.location.y + 1; y++) {
        if (y < 0 || y >= BoardSize) {
            continue;
        }

        for (int x = ship.location.x - 1; x <= ship.location.x + 1; x++) {
            if (x < 0 || x >= BoardSize) {
                continue;
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
    return false;
}

void MvpServer::AttackTargetEnemy(ShipData &ship) {
    const bool isTopPlayer = ship.owner == m_playerTop.id;
    auto &enemy = isTopPlayer ? m_playerBottom : m_playerTop;
    auto enemyShip = enemy.ships.find(ship.attackTargetID);

    // Check if adjacent
    if (enemyShip != enemy.ships.end() &&
        std::abs(enemyShip->second.location.x - ship.location.x) <= 1 &&
        std::abs(enemyShip->second.location.y - ship.location.y) <= 1) {
        // Damage target ship
        if (enemyShip->second.health < ShipInfoMap[ship.type].Damage) {
            enemyShip->second.health = 0;
        } else {
            enemyShip->second.health -= ShipInfoMap[ship.type].Damage;
        }
    } else {
        ship.action = ShipAction::None;
    }
}

void MvpServer::ProcessShips(std::map<uint16_t, ShipData> &ships) {
    for (auto &ship : ships) {
        switch (ship.second.action) {
        case ShipAction::None:
            continue;
        case ShipAction::Attack:
            if (TICK_RATE / ShipInfoMap[ship.second.type].AttackSpeed >
                static_cast<float>(m_turn - ship.second.lastActionTurn)) {
                continue;
            }
            // Base attacks all nearby
            if (ship.second.type == ShipType::Base) {
                DamageNearbyEnemies(ship.second);
            } else {
                AttackTargetEnemy(ship.second);
            }
            break;
        case ShipAction::Move:
            continue;
        default:
            break;
        }

        ship.second.lastActionTurn = m_turn;
    }
}

void MvpServer::ProcessShipsLate(std::map<uint16_t, ShipData> &ships) {
    for (auto &ship : ships) {
        switch (ship.second.action) {
        case ShipAction::None:
        case ShipAction::Attack:
            continue;
        case ShipAction::Move:
            if (ship.second.moveData.actionY == ship.second.location.y &&
                ship.second.moveData.actionX == ship.second.location.x) {
                ship.second.action = ShipAction::None;
                continue;
            }
            if (TICK_RATE / ShipInfoMap[ship.second.type].MoveSpeed >
                static_cast<float>(m_turn - ship.second.lastActionTurn)) {
                continue;
            }
            if (m_board[ship.second.moveData.actionX]
                       [ship.second.moveData.actionY] != 0) {
                continue;
            }
            m_board[ship.second.location.x][ship.second.location.y] = 0;
            ship.second.location.x = ship.second.moveData.actionX;
            ship.second.location.y = ship.second.moveData.actionY;
            m_board[ship.second.location.x][ship.second.location.y] =
                ship.second.id;
            break;
        default:
            break;
        }

        ship.second.lastActionTurn = m_turn;
    }
    return false;
}

void MvpServer::ProcessDeadShips(std::map<uint16_t, ShipData> &ships) {
    for (auto it = ships.begin(); it != ships.end();) {
        const auto ship = it->second;
        if (ship.health == 0) {
            const auto owner = ship.owner;
            (owner == m_playerTop.id ? m_playerTop : m_playerBottom).numShips--;
            m_board[ship.location.x][ship.location.y] = 0;
            it = ships.erase(it);
        } else {
            it++;
        }
    }
}

bool MvpServer::ProcessWinCondition() {
    const bool playerTopWon =
        m_playerBottom.ships[m_playerBottom.baseId].health == 0;
    const bool playerBottomWon =
        m_playerTop.ships[m_playerTop.baseId].health == 0;

    if (playerTopWon && playerBottomWon) {
        StopGame(0); // TODO: Find better value
        return true;
    }
    if (playerTopWon) {
        StopGame(m_playerTop.id);
        return true;
    }
    if (playerBottomWon) {
        StopGame(m_playerBottom.id);
        return true;
    }
    return false;
}

void MvpServer::BroadcastState() {
    Message msg;
    msg.header.id = NetworkMessageTypes::BoardUpdate;
    msg << m_turn << m_playerTop.coins << m_playerBottom.coins;

    for (auto &ship : m_playerTop.ships) {
        msg << ship.second;
    }

    for (auto &ship : m_playerBottom.ships) {
        msg << ship.second;
    }

    msg << m_playerTop.numShips << m_playerBottom.numShips;

    if (m_debug) {
        std::cout << "Turn: " << m_turn << "\n";
        std::cout << "Player 1 coins: " << m_playerTop.coins
                  << " Player 2 coins: " << m_playerBottom.coins << "\n";
        std::cout << "Player 1 ships: "
                  << static_cast<int>(m_playerTop.numShips)
                  << " Player 2 ships: "
                  << static_cast<int>(m_playerBottom.numShips) << "\n";
    }

    MessageAllClients(msg);
}
