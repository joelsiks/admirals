#pragma once
#include "objects/Sprite.hpp"

namespace admirals::mvp::objects {

class TreasureIsland : public Sprite {
public:
    TreasureIsland(const std::string &name, const Texture &source,
                   float order = 0, const Rect &bounds = Rect());

    void Render(const EngineContext &ctx) const override;
};

} // namespace admirals::mvp::objects