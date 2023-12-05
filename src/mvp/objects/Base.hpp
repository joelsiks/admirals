#pragma once
#include "objects/Sprite.hpp"

namespace admirals::mvp::objects {

class Base : public Sprite {
public:
    inline Base(const std::string &name, const Texture &source, float order = 0,
                const Rect &bounds = Rect())
        : Sprite(name, source, order, bounds) {}

    void Render(const EngineContext &ctx) const override;

    inline void SetHealth(float health) { m_health = health; }

private:
    float m_health;
};

} // namespace admirals::mvp::objects