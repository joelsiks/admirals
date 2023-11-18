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
    Vector2 offset = Vector2(r.windowWidth - GameData::GridSize,
                             r.windowHeight - GameData::GridSize) /
                     2;
    renderer::Renderer::DrawRectangle(offset, Vector2(GameData::GridSize),
                                      m_color);
}