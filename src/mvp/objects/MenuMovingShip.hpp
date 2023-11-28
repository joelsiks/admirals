#pragma once

#include "objects/Ship.hpp"
#include "objects/Sprite.hpp"

namespace admirals::mvp::objects {

class MenuMovingShip : public Sprite {
public:
    MenuMovingShip(const std::string &name, const Texture &source, float order,
                   const Rect &bounds, ShipType::ShipType type, float shipSpeed)
        : Sprite(name, source, order, bounds, Ship::ShipTypeToTexOffset(type)),
          m_shipSpeed(shipSpeed) {}

    void OnUpdate(const EngineContext &ctx) override {
        Vector2 position = this->GetPosition();

        float newX =
            position.x() + m_shipSpeed * static_cast<float>(ctx.deltaTime);
        if (newX > ctx.windowSize.x()) {
            newX = -GetSize().x();
        }

        position.SetX(newX);
        this->SetPosition(position);
    }

private:
    float m_shipSpeed;
};

} // namespace admirals::mvp::objects