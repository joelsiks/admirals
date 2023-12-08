#pragma once
#include "CommonTypes.hpp"
#include "Server.hpp"

// Number of ticks per second
#define TICK_RATE 10
#define PASSIVE_INCOME 1
#define ISLAND_INCOME 3

namespace admirals::mvp {

using namespace admirals::net;

struct PlayerData {
    uint16_t coins = 5;
    uint16_t baseId = 0;
    uint8_t id = 0;
    uint8_t numShips = 0;
    std::map<uint16_t, ShipData> ships = {};
    bool connected = false;
    bool ready = false;
};

class MvpServer : public net::Server {
public:
    MvpServer(uint16_t port, bool debug = false)
        : Server(port), m_turn(0), m_debug(debug) {}

    bool OnClientConnect(const std::shared_ptr<Connection> &) override;
    void OnClientDisconnect(const std::shared_ptr<Connection> &) override;
    void OnClientValidated(const std::shared_ptr<Connection> &client) override;
    void OnMessage(const std::shared_ptr<Connection> &client,
                   Message &message) override;

    void ProcessTurn();
    void EnterServerLoop();

private:
    bool ShipAtLocation(int x, int y);
    bool ShipExists(PlayerData &player, uint16_t id);
    void StartGame();
    void StopGame(uint8_t winner = 0);
    void PauseGame();
    void ResumeGame();
    void UpdatePlayer(uint32_t oldOwner, uint32_t newOwner);
    void PlayerReady(const std::shared_ptr<Connection> &client);
    void BuyShip(const std::shared_ptr<Connection> &client, Message &message);
    void MoveShip(const std::shared_ptr<Connection> &client, Message &message);
    void AttackShip(const std::shared_ptr<Connection> &client,
                    Message &message);
    void IncrementGoldByShipId(uint16_t shipId);
    void CheckTreasureIsland(int tx, int ty);
    void ProcessGoldGeneration();
    void DamageNearbyEnemies(ShipData &ship);
    void AttackTargetEnemy(ShipData &ship);
    void ProcessShips(std::map<uint16_t, ShipData> &ships);
    void ProcessShipsLate(std::map<uint16_t, ShipData> &ships);
    void ProcessDeadShips(std::map<uint16_t, ShipData> &ships);
    void ProcessWinCondition();
    void BroadcastState();

    uint16_t m_turn = 0;
    PlayerData m_playerTop;
    PlayerData m_playerBottom;

    // 2D array of ship ids
    uint16_t m_board[BoardSize][BoardSize] = {};

    int m_connectedPlayers = 0;
    bool m_gameStarted = false;
    bool m_gamePaused = false;

    int m_shipID = 1;

    bool m_debug = true;
};
} // namespace admirals::mvp