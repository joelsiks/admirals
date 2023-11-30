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

    static std::shared_ptr<UI::menu::Menu> startMenu;
    static std::shared_ptr<scene::Scene> startMenuScene;

    static std::unique_ptr<Engine> engine;
    static const float CellSize;
    static const float GridSize;
    static const Rect GridArea;
    static const int GridCells;
    static const float SpriteSize;
};

} // namespace admirals::mvp