#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const std::string &name, const Color &color)
    : scene::GameObject(name, 0, Vector2(0)), m_color(color) {}

void Background::OnStart(const EngineContext &c) {}

void Background::OnUpdate(const EngineContext &c) {}

void Background::Render(const EngineContext &c) const {
    const Vector2 size = Vector2(static_cast<float>(c.windowWidth),
                                 static_cast<float>(c.windowHeight));
    renderer::Renderer::DrawRectangle(Vector2(0, 0), size, m_color);
}