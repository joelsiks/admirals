#pragma once
#include <memory>

#include "Engine.hpp"

namespace admirals::mvp {

class GameData {
private:
public:
    GameData(){};
    ~GameData(){};

    static std::unique_ptr<Engine> engine;
    static const float CellSize;
    static const float GridSize;
    static const int GridCells;
    static const float SpriteSize;
};

} // namespace admirals::mvp