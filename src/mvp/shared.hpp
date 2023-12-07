#pragma once

#include <memory>

#include "Engine.hpp"
#include "UI/menu/Menu.hpp"

namespace admirals::mvp {

class GameData {
private:
public:
    GameData(){};
    ~GameData(){};

    inline static std::unique_ptr<Engine> engine = nullptr;

    inline static const size_t startMenuIdx = 0;
    inline static std::shared_ptr<Scene> startMenuScene = nullptr;
    inline static std::shared_ptr<Scene> g_sceneStore = nullptr;

    inline static const size_t gameUIIdx = 1;

    inline static std::string selectedShip;
    inline static Vector2 mousePosition;

    inline static uint16_t playerId;
    static constexpr float SpriteSize = 64;
    static constexpr float CellSize = 64;
    static constexpr float HealthBarSize = 10;
    static constexpr int GridCells = 10;
    static constexpr float GridSize = GameData::CellSize * GameData::GridCells;
    inline static const Rect GridArea =
        Rect(0, GameData::CellSize, GameData::GridSize, GameData::GridSize);
    static constexpr float TotalHeight =
        GameData::GridSize + GameData::CellSize * 2;
};

} // namespace admirals::mvp