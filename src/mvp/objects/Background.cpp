#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const std::string &name, const Color &color)
    : scene::GameObject(name, 0, Vector2(0)), m_color(color) {}

void Background::OnStart() {}

void Background::OnUpdate() {}

void Background::Render(const renderer::RendererContext &r) const {
    renderer::Renderer::DrawRectangle(
        Vector3(0), Vector2(r.windowWidth, r.windowHeight), m_color);
}