#include "Sprite.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const std::string &name, const Vector3 position,
               const Vector2 &size, const Color &color)
    : scene::GameObject(name, position), m_size(size), m_color(color) {}

Sprite::~Sprite() {}

void Sprite::OnUpdate() {}

void Sprite::OnStart() {}

void Sprite::Render(const renderer::RendererContext &r) const {
    Vector2 pos = this->GetPosition();
    renderer::Renderer::DrawRectangle(pos, m_size, m_color);
}
