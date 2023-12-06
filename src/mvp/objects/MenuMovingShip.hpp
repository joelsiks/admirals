#pragma once

#include "objects/Ship.hpp"
#include "objects/Sprite.hpp"
#include "GameData.hpp"

namespace admirals::mvp::objects {

class MenuMovingShip : public Sprite {
public:
    MenuMovingShip(const std::string &name, const Texture &source, float order,
                   const Rect &bounds, ShipType::ShipType type, float shipSpeed)
        : Sprite(name, source, order, bounds, Ship::ShipTypeToTexOffset(type)),
          m_shipSpeed(shipSpeed) {}

    void OnUpdate(const EngineContext &ctx) override {
        Sprite::OnUpdate(ctx);

        float newX = m_boundingBox.PositionX() +
                     m_shipSpeed * static_cast<float>(ctx.deltaTime);
        if (newX > static_cast<float>(GameData::GridCells)) {
            newX = -1;
        }

        m_boundingBox.SetPositionX(newX);
    }

private:
    float m_shipSpeed;
};

} // namespace admirals::mvp::objects