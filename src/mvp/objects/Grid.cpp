#include "Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const std::string &name, const Color &color)
    : GridObject(name, 1000), m_color(color) {}

void Grid::Render(const EngineContext &) const {
    const Vector2 offset = GetPosition();
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float x = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(
            Vector2(x, 0) * m_scale + offset,
            Vector2(x, GameData::GridSize) * m_scale + offset, m_color);
    }
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float y = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(
            Vector2(0, y) * m_scale + offset,
            Vector2(GameData::GridSize, y) * m_scale + offset, m_color);
    }
}