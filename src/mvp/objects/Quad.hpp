#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Quad : public scene::GameObject {
public:
    Quad(const std::string &name, const Vector3 &position, const Vector2 &size,
         const Color &color);

    void OnUpdate(const EngineContext &c) override;
    void OnStart(const EngineContext &c) override;
    void Render(const EngineContext &c) const override;

private:
    Color m_color;
    Vector2 m_size;
};

} // namespace admirals::mvp::objects