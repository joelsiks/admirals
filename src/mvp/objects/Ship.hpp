#pragma once
#include "Sprite.hpp"

namespace admirals::mvp::objects {

class Ship : public Sprite {
public:
    Ship(const std::string &name, const Vector3 position, const Vector2 &size,
         const Texture &source, const Vector2 &texOffset = Vector2(0));

    void OnUpdate() override;
    void OnStart() override;
};

} // namespace admirals::mvp::objects