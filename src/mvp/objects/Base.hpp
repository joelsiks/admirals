#pragma once
#include "objects/Ship.hpp"

namespace admirals::mvp::objects {

class Base : public Ship {
public:
    Base(const ShipData &data, const Vector2 &size, const Texture &source);
    ~Base() override;

    inline void OnUpdate(const EngineContext &ctx) override {}
    inline void OnClick(events::MouseClickEventArgs &args) override {}
    void Render(const EngineContext &ctx) const override;

    inline void Select() override {}
    inline void DeSelect() override {}
};

} // namespace admirals::mvp::objects