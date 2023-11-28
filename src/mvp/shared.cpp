#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp;

std::shared_ptr<UI::menu::Menu> GameData::startMenu = nullptr;
std::shared_ptr<scene::Scene> GameData::startMenuScene = nullptr;

std::unique_ptr<Engine> GameData::engine = nullptr;
const float GameData::CellSize = 64;
const int GameData::GridCells = 10;
const float GameData::GridSize = GameData::CellSize * GameData::GridCells;
const float GameData::SpriteSize = 64;