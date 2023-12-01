#include "Quad.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Quad::Quad(const std::string &name, float order, const Rect &bounds,
           const Color &color)
    : GameObject(name, order, bounds), m_color(color) {}

void Quad::Render(const EngineContext &ctx) const {
    const Vector2 pos = this->GetPosition();
    const Vector2 offset =
        (ctx.windowSize -
         Vector2(GameData::GridSize,
                 GameData::GridSize + 2 * GameData::CellSize)) /
        2.f;
    renderer::Renderer::DrawRectangle(pos + offset, GetSize(), m_color);
}
