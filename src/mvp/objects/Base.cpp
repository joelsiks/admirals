#include "Base.hpp"
#include "commontypes.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

void Base::Render(const EngineContext &ctx) const {
    Sprite::Render(ctx);

    const auto position = GetPosition();
    const auto size = GetSize();

    // Health Bar
    const float healthPercentage = m_health / static_cast<float>(BaseMaxHealth);

    const Vector2 healthOrigin =
        position + Vector2(0, size.y() - GameData::HealthBarSize);
    renderer::Renderer::DrawRectangle(
        healthOrigin, Vector2(size.x(), GameData::HealthBarSize), Color::WHITE);
    renderer::Renderer::DrawRectangle(
        healthOrigin + Vector2(size.x() * (1 - healthPercentage), 0),
        Vector2(size.x() * healthPercentage, GameData::HealthBarSize),
        Color::BLACK);
}