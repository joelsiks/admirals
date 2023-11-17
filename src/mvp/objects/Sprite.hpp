#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Sprite : public scene::GameObject {
public:
    // replace color with texture later
    Sprite(const std::string &name, const Vector3 position, const Vector2 &size,
           const Color &color);
    ~Sprite();

    void OnUpdate() override;
    void OnStart() override;
    void Render(const renderer::RendererContext &r) const override;

private:
    Color m_color;
    Vector2 m_size;
};

} // namespace admirals::mvp::objects