#include "Base.hpp"
#include "CommonTypes.hpp"
#include "GameData.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Base::Base(const ShipData &data, const Vector2 &size, const Texture &source)
    : Ship(data, size, source) {}

Base::~Base() {}

void Base::Render(const EngineContext &) const {
    renderer::Renderer::DrawSprite(m_source, GetPosition(), m_texOffset,
                                   m_texSize, GetSize() / m_texSize);

    DrawHealthBar();
}