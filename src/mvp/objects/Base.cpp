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

    const auto position = GetPosition();
    const auto size = GetSize();

    // Health Bar
    const float healthPercentage =
        static_cast<float>(GetHealth()) /
        static_cast<float>(ShipInfoMap[ShipType::Base].Health);

    const Vector2 healthOrigin =
        position + Vector2(0, size.y() - GameData::HealthBarSize);
    renderer::Renderer::DrawRectangle(
        healthOrigin, Vector2(size.x(), GameData::HealthBarSize), Color::GREY);
    renderer::Renderer::DrawRectangle(
        healthOrigin,
        Vector2(size.x() * healthPercentage, GameData::HealthBarSize),
        Color::BLACK);
}