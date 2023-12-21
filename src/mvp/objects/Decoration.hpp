#pragma once
#include "objects/Sprite.hpp"

namespace admirals::mvp::objects {

class Decoration : public Sprite {
public:
    enum DecorationType {
        None = 0,
        Water = 1,
        Beach = 2,
        Grass = 3,
        Ring = 4,
        Green = 5,
    };

    Decoration(const std::string &name, const Texture &source, int type,
               Vector2 spriteOffset = Vector2(), float order = 0,
               const Rect &bounds = Rect());

    void Render(const EngineContext &ctx) const override;

    std::vector<Vector2> DecorationTypeToTexOffset(int type,
                                                   Vector2 spriteOffset);
};

} // namespace admirals::mvp::objects