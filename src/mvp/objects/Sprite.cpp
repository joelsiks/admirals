#include "objects/Sprite.hpp"
#include "Animator.hpp"
#include "GameData.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const std::string &name, const Texture &source, float order,
               const Rect &bounds, const Vector2 &texOffset,
               const Vector2 &texSize)
    : GridObject(name, order, bounds), m_source(source), m_texSize(texSize),
      m_texOffset({texOffset}) {}

Sprite::Sprite(const std::string &name, const Texture &source, float order,
               const Rect &bounds, const std::vector<Vector2> &texOffset,
               const Vector2 &texSize)
    : GridObject(name, order, bounds), m_source(source), m_texSize(texSize),
      m_texOffset(texOffset) {}

void Sprite::Render(const EngineContext &) const {
    Vector2 frame = Animator::GetAnimationFrame(m_texOffset);
    renderer::Renderer::DrawSprite(m_source, GetPosition(), frame, m_texSize,
                                   GetSize() / m_texSize);
}
