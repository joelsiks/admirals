#pragma once
#include "Server.hpp"
#include "commontypes.hpp"

// Number of ticks per second
#define TICK_RATE 1

#define BOARD_SIZE 10

#define PASSIVE_INCOME 1
#define ISLAND_INCOME 3
#define STARTING_COINS 20

namespace admirals::mvp {

struct PlayerData {
    uint16_t coins = STARTING_COINS;
    uint16_t baseHealth = BaseMaxHealth;
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

    bool OnClientConnect(std::shared_ptr<admirals::net::Connection>) override;
    void
        OnClientDisconnect(std::shared_ptr<admirals::net::Connection>) override;
    void OnClientValidated(
        std::shared_ptr<admirals::net::Connection> client) override;
    void OnMessage(std::shared_ptr<admirals::net::Connection> client,
                   admirals::net::Message &message) override;

    void ProcessTurn();
    void EnterServerLoop(bool& stopServer);

private:
    bool ShipAtLocation(int x, int y);
    bool ShipExists(PlayerData &player, uint16_t id);
    void StartGame();
    void StopGame(uint8_t winner = 0);
    void PauseGame();
    void ResumeGame();
    void ResetState();
    void UpdatePlayer(uint32_t oldOwner, uint32_t newOwner);
    void PlayerReady(std::shared_ptr<admirals::net::Connection> client);
    void BuyShip(std::shared_ptr<admirals::net::Connection> &client,
                 admirals::net::Message &message);
    void MoveShip(std::shared_ptr<admirals::net::Connection> client,
                  admirals::net::Message &message);
    void AttackShip(std::shared_ptr<admirals::net::Connection> client,
                    admirals::net::Message &message);
    void IncrementGoldByShipId(uint16_t shipId);
    void CheckTreasureIsland(int tx, int ty);
    void ProcessGoldGeneration();
    void DamageNearbyEnemies(admirals::mvp::ShipData &ship);
    void ProcessShips(std::map<uint16_t, admirals::mvp::ShipData> &ships);
    void ProcessDeadShips(std::map<uint16_t, admirals::mvp::ShipData> &ships);
    void BroadcastState();

    uint16_t m_turn = 0;
    PlayerData m_playerTop;
    PlayerData m_playerBottom;

    // 2D array of ship ids
    uint16_t m_board[BOARD_SIZE][BOARD_SIZE] = {};

    int m_connectedPlayers = 0;
    bool m_gameStarted = false;
    bool m_gamePaused = false;

    int m_shipID = 1;

    bool m_debug = true;
};
} // namespace admirals::mvp