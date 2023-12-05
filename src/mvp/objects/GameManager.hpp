#pragma once
#include "Base.hpp"
#include "GameObject.hpp"
#include "commontypes.hpp"
#include "events/EventSystem.hpp"
#include "objects/Ship.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class NetworkManager;
class MenuManager;

class CoinsChangedEventArgs : public events::EventArgs {
public:
    CoinsChangedEventArgs(int ctx) : coins(ctx) {}
    const int coins;
};

class PlayerIdChangedEventArgs : public events::EventArgs {
public:
    PlayerIdChangedEventArgs(uint32_t id) : playerId(id) {}
    const uint32_t playerId;
};

class GameManager : public GameObject {
public:
    GameManager(const std::string &name, const Texture &m_atlas);
    ~GameManager();

    events::EventSystem<CoinsChangedEventArgs> onCoinsChanged;
    events::EventSystem<PlayerIdChangedEventArgs> onPlayerIdChanged;

    void OnStart(const EngineContext &ctx) override;
    void OnUpdate(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override {}

    bool StartAndConnectToServer(uint16_t port = 60000, size_t maxTries = -1);

    bool ConnectToServer(const std::string &ip = "127.0.0.1",
                         uint16_t port = 60000, size_t maxTries = -1);

    void StartGame() { m_gameStarted = true; }
    void StopGame(uint8_t winner);
    void AbortGame();
    void PauseGame();
    void ResumeGame();
    bool GameStarted() const { return m_gameStarted; }

    uint32_t GetPlayerId() const { return m_playerId; }
    void SetPlayerId(uint32_t id) {
        m_playerId = id;
        PlayerIdChangedEventArgs args(m_playerId);
        onPlayerIdChanged.Invoke(this, args);
    }

    bool GetIsTopPlayer() const { return m_isTopPlayer; }
    void SetIsTopPlayer(bool isTopPlayer) { m_isTopPlayer = isTopPlayer; }

    void SetBases(const std::shared_ptr<Base> &baseTop,
                  const std::shared_ptr<Base> &baseBottom) {
        m_baseTop = baseTop;
        m_baseBottom = baseBottom;
    }

    void BuyShip(uint8_t type);
    void MoveShip(uint16_t id, uint8_t x, uint8_t y);
    void AttackShip(uint16_t id, uint16_t targetId);

    void UpdateBoard(int turn, int coins, int baseHealth, int enemyBaseHealth,
                     const std::map<uint16_t, ShipData> &ships);

private:
    void ModifyShips(const std::map<uint16_t, ShipData> &new_ships);
    void ShipChangeEventHandler(void *sender, admirals::events::EventArgs e);

    bool m_testActionDone = false;
    bool m_gameStarted = false;
    bool m_gamePaused = false;

    int m_turn = 0;
    int m_coins = 0;
    int m_shipsP1 = 0;
    int m_shipsP2 = 0;
    int m_baseHealth = 0;
    int m_enemyBaseHealth = 0;
    uint32_t m_playerId = 0;
    bool m_isTopPlayer = false;

    std::map<uint16_t, std::shared_ptr<Ship>> m_ships;

    const Texture &m_atlas;
    Vector2 m_cellSize = Vector2(GameData::CellSize);

    std::shared_ptr<NetworkManager> m_networkManager;
    std::shared_ptr<MenuManager> m_menuManager;
    std::shared_ptr<Base> m_baseTop = nullptr;
    std::shared_ptr<Base> m_baseBottom = nullptr;

    bool m_debug = true;
};

} // namespace admirals::mvp::objects
