#include "Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const std::string &name, const Color &color)
    : scene::GameObject(name, 1000, Vector2(0, 0)), m_color(color) {}

void Grid::OnStart(const EngineContext &c) {}

void Grid::OnUpdate(const EngineContext &c) {}

void Grid::Render(const EngineContext &c) const {
    const Vector2 offset = (c.windowSize - Vector2(GameData::GridSize)) / 2.f;
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float x = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(Vector2(x, 0) + offset,
                                     Vector2(x, GameData::GridSize) + offset,
                                     m_color);
    }
    for (int i = 0; i <= GameData::GridCells; i++) {
        const float y = static_cast<float>(i) * GameData::CellSize;
        renderer::Renderer::DrawLine(Vector2(0, y) + offset,
                                     Vector2(GameData::GridSize, y) + offset,
                                     m_color);
    }
}