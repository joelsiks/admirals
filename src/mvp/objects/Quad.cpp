#include "Quad.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Quad::Quad(const std::string &name, const Vector3 position, const Vector2 &size,
           const Color &color)
    : scene::GameObject(name, position), m_size(size), m_color(color) {}

void Quad::OnUpdate() {}

void Quad::OnStart() {}

void Quad::Render(const renderer::RendererContext &r) const {
    Vector2 pos = this->GetPosition();
    Vector2 offset =
        Vector2(r.windowWidth - GameData::GridSize,
                r.windowHeight - GameData::GridSize - 2 * GameData::CellSize) /
        2;
    renderer::Renderer::DrawRectangle(pos + offset, m_size, m_color);
}
