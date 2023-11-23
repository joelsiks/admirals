#include "Sprite.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Sprite::Sprite(const std::string &name, const Vector3 &position,
               const Vector2 &size, const Texture &source,
               const Vector2 &texOffset, const Vector2 &texSize)
    : Sprite(name, position.z(), position.xy(), size, source, texOffset,
             texSize) {}

Sprite::Sprite(const std::string &name, float order, const Vector2 &position,
               const Vector2 &size, const Texture &source,
               const Vector2 &texOffset, const Vector2 &texSize)
    : scene::GameObject(name, order, position), m_size(size), m_source(source),
      m_texSize(texSize), m_texOffset(texOffset) {}

void Sprite::OnUpdate(Context &c) {}

void Sprite::OnStart(Context &c) {}

void Sprite::Render(const Context &c) const {
    const Vector2 org = this->CalcOrigin();
    const Vector2 offset =
        Vector2(static_cast<float>(c.windowWidth) - GameData::GridSize,
                static_cast<float>(c.windowHeight) - GameData::GridSize -
                    2 * GameData::CellSize) /
        2;
    renderer::Renderer::DrawSprite(m_source, org + offset, m_texOffset,
                                   m_texSize, m_size / m_texSize);
}
