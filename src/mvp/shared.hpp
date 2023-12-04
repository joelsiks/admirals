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

    static const size_t startMenuIdx;
    static std::shared_ptr<Scene> startMenuScene;

    static const size_t gameUIIdx;

    static std::unique_ptr<Engine> engine;
    static const float CellSize;
    static const float GridSize;
    static const int GridCells;
    static const float SpriteSize;
};

} // namespace admirals::mvp