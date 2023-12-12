#pragma once

#include <memory>
#include <sstream>

#include "Engine.hpp"
#include "objects/SelectionManager.hpp"

namespace admirals::mvp {

class GameData {
private:
public:
    GameData() {}
    ~GameData() {}

    inline static std::unique_ptr<Engine> engine = nullptr;
    inline static std::shared_ptr<NavMesh> navMesh = nullptr;

    inline static std::shared_ptr<Scene> StartMenuScene = nullptr;
    inline static std::shared_ptr<Scene> SceneStore = nullptr;

    static constexpr size_t StartMenuIdx = 0;
    static constexpr size_t GameUIIdx = 1;
    static constexpr size_t ConnectMenuIdx = 2;

    inline static std::shared_ptr<objects::SelectionManager> Selection;
    inline static Vector2 MousePosition;

    inline static uint16_t PlayerId = 0;
    static constexpr float SpriteSize = 64;
    static constexpr float CellSize = 64;
    static constexpr float HealthBarSize = 10;
    static constexpr int GridCells = 10;
    static constexpr float GridSize = GameData::CellSize * GameData::GridCells;
    static constexpr Rect GridArea =
        Rect(0, GameData::CellSize, GameData::GridSize, GameData::GridSize);
    static constexpr float TotalHeight =
        GameData::GridSize + GameData::CellSize * 2;
};

template <typename T>
std::string DecimalToString(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

} // namespace admirals::mvp