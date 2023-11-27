#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const std::string &name, const Color &color)
    : scene::GameObject(name, 0, Vector2(0)), m_color(color) {}

void Background::OnStart(const EngineContext &ctx) {}

void Background::OnUpdate(const EngineContext &ctx) {}

void Background::Render(const EngineContext &ctx) const {
    renderer::Renderer::DrawRectangle(Vector2(0, 0), ctx.windowSize, m_color);
}