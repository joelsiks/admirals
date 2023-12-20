#include "TreasureIsland.hpp"
#include "DataObjects.hpp"

using namespace ::admirals;
using namespace ::admirals::mvp;
using namespace ::admirals::mvp::objects;

TreasureIsland::TreasureIsland(const std::string &name, const Texture &source,
                               float order, const Rect &bounds)
    : Sprite(name, source, order, bounds,
             Vector2(GameData::SpriteSize, GameData::SpriteSize)) {}

void TreasureIsland::Render(const EngineContext &ctx) const {
    Sprite::Render(ctx);
}