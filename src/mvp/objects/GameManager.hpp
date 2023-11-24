#pragma once
#include "GameObject.hpp"
#include "Ship.hpp"
#include "commontypes.hpp"
#include "events/EventSystem.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class NetworkManager;

class CoinsChangesEventArgs : public events::EventArgs {
public:
    CoinsChangesEventArgs(int c) : coins(c) {}
    const int coins;
};

class GameManager : public scene::GameObject {
public:
    GameManager(const std::string &name);
    ~GameManager();

    events::EventSystem<CoinsChangesEventArgs> onCoinsChanged;

    void OnStart(const EngineContext &c) override;
    void OnUpdate(const EngineContext &c) override;
    void Render(const EngineContext &c) const override {}

    void StartGame() { m_gameStarted = true; }
    void StopGame();
    bool GameStarted() const { return m_gameStarted; }

    uint32_t GetPlayerId() const { return m_playerId; }
    void SetPlayerId(uint32_t id) { m_playerId = id; }

    void BuyShip(uint8_t type);
    void MoveShip(uint16_t id, int x, int y);
    void AttackShip(uint16_t id, uint16_t targetId);

    void UpdateBoard(int turn, int coins, int baseHealth, int enemyBaseHealth,
                     const std::map<uint16_t, ShipData> &ships);

private:
    void ModifyShips(const std::map<uint16_t, ShipData> &new_ships);
    void ShipChangeEventHandler(void *sender, admirals::events::EventArgs e);

    bool m_testActionDone = false;
    bool m_gameStarted = false;

    int m_turn = 0;
    int m_coins = 0;
    int m_shipsP1 = 0;
    int m_shipsP2 = 0;
    int m_baseHealth = 0;
    int m_enemyBaseHealth = 0;
    uint32_t m_playerId = 0;

    std::map<uint16_t, std::shared_ptr<Ship>> m_ships;

    Texture m_atlas =
        Texture::LoadFromPath("assets/admirals_texture_atlas.png");
    Vector2 m_cellSize = Vector2(GameData::CellSize);

    std::shared_ptr<NetworkManager> m_networkManager;

    bool m_debug = true;
};

} // namespace admirals::mvp::objects
