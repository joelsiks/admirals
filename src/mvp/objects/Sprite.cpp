#include "Sprite.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const Vector3 position, const Vector2 &size, const Color &color)
    : GameObject(position), m_size(size), m_color(color) {}

Sprite::~Sprite() {}

void Sprite::onUpdate() {}

void Sprite::onStart() {}

void Sprite::render() const {
    Vector2 pos = this->position();
    renderer::Renderer::drawRectangle(pos, m_size, m_color);
}
