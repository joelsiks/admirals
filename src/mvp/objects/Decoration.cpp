#include "Decoration.hpp"
#include "DataObjects.hpp"

using namespace ::admirals;
using namespace ::admirals::mvp;
using namespace ::admirals::mvp::objects;

Decoration::Decoration(const std::string &name, const Texture &source, int type,
                       Vector2 spriteOffset, float order, const Rect &bounds)
    : Sprite(name, source, order, bounds,
             Decoration::DecorationTypeToTexOffset(type, spriteOffset)) {}

void Decoration::Render(const EngineContext &ctx) const { Sprite::Render(ctx); }

std::vector<Vector2>
Decoration::DecorationTypeToTexOffset(int type, Vector2 spriteOffset) {

    std::vector<Vector2> textureOffset;
    switch (type) {
    case DecorationType::Water:
        textureOffset.push_back((Vector2(8, 3) + spriteOffset) *
                                GameData::SpriteSize);
        textureOffset.push_back((Vector2(8, 7) + spriteOffset) *
                                GameData::SpriteSize);
        textureOffset.push_back((Vector2(8, 11) + spriteOffset) *
                                GameData::SpriteSize);
        textureOffset.push_back((Vector2(8, 15) + spriteOffset) *
                                GameData::SpriteSize);
        break;
    case DecorationType::Beach:
        textureOffset.push_back(Vector2(15, 1) * GameData::SpriteSize);
        break;
    case DecorationType::Grass:
        break;
    case DecorationType::Ring:
        textureOffset.push_back(Vector2(13, 0) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(14, 0) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(15, 0) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(16, 0) * GameData::SpriteSize);
        break;
    case DecorationType::Green:
        textureOffset.push_back(Vector2(0, 7) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(1, 7) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(2, 7) * GameData::SpriteSize);
        textureOffset.push_back(Vector2(3, 7) * GameData::SpriteSize);
        break;
    default:
        textureOffset.push_back(Vector2(0, 0));
        break;
    }
    return textureOffset;
}