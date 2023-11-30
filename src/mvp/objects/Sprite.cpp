#include "Sprite.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const std::string &name, const Texture &source, float order,
               const Rect &bounds, const Vector2 &texOffset,
               const Vector2 &texSize)
    : GridObject(name, order, bounds), m_source(source), m_texSize(texSize),
      m_texOffset(texOffset) {}

void Sprite::Render(const EngineContext &) const {
    renderer::Renderer::DrawSprite(m_source, GetPosition(), m_texOffset,
                                   m_texSize, GetSize() / m_texSize);
}
