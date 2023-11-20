#pragma once
#include "GameObject.hpp"
#include "Ship.hpp"
#include "UI/TextElement.hpp"
#include "commontypes.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class NetworkManager;

class GameManager : public scene::GameObject {
public:
    GameManager(const std::string &name);
    ~GameManager();

    void OnStart() override;
    void OnUpdate() override;
    void Render(const renderer::RendererContext &r) const override {}

    void StartGame() {
        m_gameStarted = true;
        printf("GameManager::StartGame()\n");
    }
    void StopGame() { m_gameStarted = false; }
    bool GameStarted() const { return m_gameStarted; }

    uint32_t GetPlayerId() const { return m_playerId; }
    void SetPlayerId(uint32_t id) { m_playerId = id; }

    void UpdateBoard(int turn, int coins, int baseHealth, int enemyBaseHealth,
                     const std::map<uint16_t, ShipData> &ships);

private:
    void ModifyShips(const std::map<uint16_t, ShipData> &new_ships);

private:
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

    std::shared_ptr<admirals::UI::TextElement> m_debugText;
    bool m_debug = false;
};

} // namespace admirals::mvp::objects
