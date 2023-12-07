#include "Base.hpp"
#include "CommonTypes.hpp"
#include "GameData.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Base::Base(const ShipData &data, const Vector2 &size, const Texture &source)
    : Ship(data, size, source) {}

Base::~Base() {}

void Base::OnUpdate(const EngineContext &ctx) {
    m_timer += ctx.deltaTime;
    if (m_timer > 0.3) {
        m_timer = 0;
        m_texIndex += 1;
        m_texIndex = m_texIndex % m_texOffset.size();
    }
}

void Base::Render(const EngineContext &) const {
    renderer::Renderer::DrawSprite(m_source, GetPosition(),
                                   m_texOffset[m_texIndex], m_texSize,
                                   GetSize() / m_texSize);

    DrawHealthBar();
}