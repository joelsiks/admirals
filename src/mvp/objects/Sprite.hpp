#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Sprite : public scene::GameObject {
public:
    // replace color with texture later
    Sprite(const Vector3 position, const Vector2 &size, const Color &color);
    ~Sprite();

    void onUpdate() override;
    void onStart() override;
    void render(const renderer::RendererContext &r) override;

private:
    Color m_color;
    Vector2 m_size;
};

} // namespace admirals::mvp::objects