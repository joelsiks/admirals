#pragma once
#include "CommonTypes.hpp"
#include "Server.hpp"

// Number of ticks per second
#define TICK_RATE 10
#define PASSIVE_INCOME 1
#define ISLAND_INCOME 3
#define STARTING_COINS 20

namespace admirals::mvp {

struct PlayerData {
    uint16_t coins = STARTING_COINS;
    uint16_t baseId = 0;
    uint8_t id = 0;
    uint8_t numShips = 0;
    std::map<uint16_t, ShipData> ships = {};
    bool connected = false;
    bool ready = false;
};

class MvpServer : public admirals::net::Server {
public:
    MvpServer(uint16_t port, bool debug = false)
        : Server(port), m_turn(0), m_debug(debug) {}

    bool OnClientConnect(
        const std::shared_ptr<admirals::net::Connection> &) override;
    void OnClientDisconnect(
        const std::shared_ptr<admirals::net::Connection> &) override;
    void OnClientValidated(
        const std::shared_ptr<admirals::net::Connection> &client) override;
    void OnMessage(const std::shared_ptr<admirals::net::Connection> &client,
                   admirals::net::Message &message) override;

    void ProcessTurn();
    void EnterServerLoop(bool &stopServer);

private:
    bool ShipAtLocation(int x, int y);
    bool ShipExists(PlayerData &player, uint16_t id);
    void StartGame();
    void StopGame(uint8_t winner = 0);
    void PauseGame();
    void ResumeGame();
    void ResetState();
    void UpdatePlayer(uint32_t oldOwner, uint32_t newOwner);
    void PlayerReady(const std::shared_ptr<admirals::net::Connection> &client);
    void BuyShip(const std::shared_ptr<admirals::net::Connection> &client,
                 admirals::net::Message &message);
    void MoveShip(const std::shared_ptr<admirals::net::Connection> &client,
                  admirals::net::Message &message);
    void AttackShip(const std::shared_ptr<admirals::net::Connection> &client,
                    admirals::net::Message &message);
    void IncrementGoldByShipId(uint16_t shipId);
    void CheckTreasureIsland(int tx, int ty);
    void ProcessGoldGeneration();
    void DamageNearbyEnemies(ShipData &ship);
    void AttackTargetEnemy(ShipData &ship);
    void ProcessShips(std::map<uint16_t, ShipData> &ships);
    void ProcessShipsLate(std::map<uint16_t, ShipData> &ships);
    void ProcessDeadShips(std::map<uint16_t, ShipData> &ships);
    bool ProcessWinCondition();
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