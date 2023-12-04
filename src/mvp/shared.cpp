#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp;

const size_t GameData::startMenuIdx = 0;
std::shared_ptr<Scene> GameData::startMenuScene = nullptr;

const size_t GameData::gameUIIdx = 1;

std::unique_ptr<Engine> GameData::engine = nullptr;
const float GameData::CellSize = 64;
const int GameData::GridCells = 10;
const float GameData::GridSize = GameData::CellSize * GameData::GridCells;
const float GameData::SpriteSize = 64;