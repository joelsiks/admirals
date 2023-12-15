#include "Background.hpp"
#include "GameData.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const std::string &name, const Color &color,
                       const Color &outerColor)
    : GridObject(name, 0, Rect(0, GameData::GridArea.Size())), m_color(color),
      m_outerColor(outerColor) {}

void Background::Render(const EngineContext &ctx) const {
    renderer::Renderer::DrawRectangle(0, ctx.windowSize, m_outerColor);
    renderer::Renderer::DrawRectangle(GetBoundingBox(), m_color);
}