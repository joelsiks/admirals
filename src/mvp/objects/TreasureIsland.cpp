#include "TreasureIsland.hpp"
#include "DataObjects.hpp"

using namespace ::admirals;
using namespace ::admirals::mvp;
using namespace ::admirals::mvp::objects;

const Color GOLD = Color::FromHEX("#FFD700");

TreasureIsland::TreasureIsland(const std::string &name, const Texture &source,
                               float order, const Rect &bounds)
    : Sprite(name, source, order, bounds,
             {Vector2(GameData::SpriteSize, GameData::SpriteSize * 2)}) {}

void TreasureIsland::Render(const EngineContext &ctx) const {
    Sprite::Render(ctx);
}