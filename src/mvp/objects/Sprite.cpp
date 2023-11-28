#include "Sprite.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const std::string &name, const Texture &source, float order,
               const Rect &bounds, const Vector2 &texOffset,
               const Vector2 &texSize)
    : scene::GameObject(name, order, bounds), m_source(source),
      m_texSize(texSize), m_texOffset(texOffset) {}

void Sprite::Render(const EngineContext &ctx) const {
    const Vector2 org = this->CalcOrigin();
    const Vector2 offset =
        (ctx.windowSize -
         Vector2(GameData::GridSize,
                 GameData::GridSize + 2 * GameData::CellSize)) /
        2.f;
    renderer::Renderer::DrawSprite(m_source, org + offset, m_texOffset,
                                   m_texSize, GetSize() / m_texSize);
}
