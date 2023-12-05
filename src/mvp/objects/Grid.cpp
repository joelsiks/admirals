#include "objects/Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const std::string &name, const Color &color)
    : GridObject(name, 1000), m_color(color) {}

void Grid::Render(const EngineContext &) const {
    const Vector2 offset = GetPosition();
    const float scale = GridObject::GetGridScale();
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float x = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(
            Vector2(x, 0) * scale + offset,
            Vector2(x, GameData::GridSize) * scale + offset, m_color);
    }
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float y = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(
            Vector2(0, y) * scale + offset,
            Vector2(GameData::GridSize, y) * scale + offset, m_color);
    }
}