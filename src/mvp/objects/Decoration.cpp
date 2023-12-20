#include "Decoration.hpp"
#include "DataObjects.hpp"

using namespace ::admirals;
using namespace ::admirals::mvp;
using namespace ::admirals::mvp::objects;

Decoration::Decoration(const std::string &name, const Texture &source, int type,
                       float order, const Rect &bounds)
    : Sprite(name, source, order, bounds,
             Decoration::DecorationTypeToTexOffset(type)) {}

void Decoration::Render(const EngineContext &ctx) const { Sprite::Render(ctx); }

std::vector<Vector2> Decoration::DecorationTypeToTexOffset(int type) {

    std::vector<Vector2> textureOffset;
    switch (type) {
    case DecorationType::Water:
        textureOffset.push_back(Vector2(0, GameData::SpriteSize * 14));
        textureOffset.push_back(
            Vector2(GameData::SpriteSize, GameData::SpriteSize * 14));
        textureOffset.push_back(
            Vector2(GameData::SpriteSize * 2, GameData::SpriteSize * 14));
        textureOffset.push_back(
            Vector2(GameData::SpriteSize * 3, GameData::SpriteSize * 14));
        break;
    case DecorationType::Grass:
        textureOffset.push_back(Vector2(GameData::SpriteSize * 17, 0));
        break;
    default:
        textureOffset.push_back(Vector2(0, GameData::SpriteSize));
        break;
    }
    return textureOffset;
}