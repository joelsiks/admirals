#include "Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const std::string &name, const Color &color)
    : scene::GameObject(name, 1000, Vector2(0, 0)), m_color(color) {}

void Grid::OnStart(Context &c) {}

void Grid::OnUpdate(Context &c) {}

void Grid::Render(const Context &c) const {
    Vector2 offset = Vector2(c.windowWidth - GameData::GridSize,
                             c.windowHeight - GameData::GridSize) /
                     2;
    for (int i = 0; i <= GameData::GridCells; i++) {
        float x = i * GameData::CellSize;
        renderer::Renderer::DrawLine(Vector2(x, 0) + offset,
                                     Vector2(x, GameData::GridSize) + offset,
                                     m_color);
    }
    for (int i = 0; i <= GameData::GridCells; i++) {
        float y = i * GameData::CellSize;
        renderer::Renderer::DrawLine(Vector2(0, y) + offset,
                                     Vector2(GameData::GridSize, y) + offset,
                                     m_color);
    }
}