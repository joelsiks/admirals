#pragma once
#include "objects/Sprite.hpp"

namespace admirals::mvp::objects {

class Decoration : public Sprite {
public:
    enum DecorationType {
        None = 0,
        Water = 1,
        Grass = 2,
    };

    Decoration(const std::string &name, const Texture &source, int type,
               float order = 0, const Rect &bounds = Rect());

    void Render(const EngineContext &ctx) const override;

    std::vector<Vector2> DecorationTypeToTexOffset(int type);
};

} // namespace admirals::mvp::objects