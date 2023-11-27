#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const std::string &name, const Color &color)
    : scene::GameObject(name), m_color(color) {}

void Background::Render(const EngineContext &ctx) const {
    renderer::Renderer::DrawRectangle(Vector2(0, 0), ctx.windowSize, m_color);
}