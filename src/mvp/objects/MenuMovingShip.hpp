#pragma once

#include <chrono>

#include "Ship.hpp"
#include "Sprite.hpp"
#include "commontypes.hpp"

namespace admirals::mvp::objects {

class MenuMovingShip : public Sprite {
public:
    MenuMovingShip(const std::string &name, const Vector2 &position,
                   const Vector2 &size, const Texture &source,
                   ShipType::ShipType type, float shipSpeed)
        : Sprite(name, 3, position, size, source,
                 Ship::ShipTypeToTexOffset(type)),
          m_size(size), m_shipSpeed(shipSpeed) {}

    void OnStart(const EngineContext &c) override {
        m_time = std::chrono::high_resolution_clock::now();
    }

    void OnUpdate(const EngineContext &c) override {
        Vector2 position = this->GetPosition();
        Vector2 windowSize = GameData::engine->GetWindowSize();

        float newX =
            position.x() + m_shipSpeed * static_cast<float>(c.deltaTime);
        if (newX > windowSize[0]) {
            newX = -m_size.x();
        }

        position.SetX(newX);
        this->SetPosition(position);
    }

private:
    Vector2 m_size;
    float m_windowWidth;
    std::chrono::_V2::system_clock::time_point m_time;

    float m_shipSpeed;
};

} // namespace admirals::mvp::objects