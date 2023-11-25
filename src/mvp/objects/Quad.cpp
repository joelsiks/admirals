#include "Quad.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Quad::Quad(const std::string &name, const Vector3 &position,
           const Vector2 &size, const Color &color)
    : scene::GameObject(name, position), m_size(size), m_color(color) {}

void Quad::OnUpdate(const EngineContext &c) {}

void Quad::OnStart(const EngineContext &c) {}

void Quad::Render(const EngineContext &c) const {
    const Vector2 pos = this->GetPosition();
    const Vector2 offset =
        (c.windowSize - Vector2(GameData::GridSize,
                                GameData::GridSize + 2 * GameData::CellSize)) /
        2.f;
    renderer::Renderer::DrawRectangle(pos + offset, m_size, m_color);
}
