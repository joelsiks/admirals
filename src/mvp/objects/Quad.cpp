#include "objects/Quad.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Quad::Quad(const std::string &name, float order, const Rect &bounds,
           const Color &color)
    : GridObject(name, order, bounds), m_color(color) {}

void Quad::Render(const EngineContext &) const {
    renderer::Renderer::DrawRectangle(GetBoundingBox(), m_color);
}
