#pragma once
#include "objects/Ship.hpp"

namespace admirals::mvp::objects {

class Base : public Ship {
public:
    Base(const ShipData &data, const Vector2 &size, const Texture &source);
    ~Base() override;

    void OnUpdate(const EngineContext &ctx) override;
    inline void OnClick(events::MouseClickEventArgs &args) override {}
    void Render(const EngineContext &ctx) const override;

private:
    float m_timer = 0;
};

} // namespace admirals::mvp::objects