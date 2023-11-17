#include "Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const Color &color)
    : scene::GameObject(Vector3(0, 0, 1000)), m_color(color) {}

Grid::~Grid() {}

void Grid::onStart() {}

void Grid::onUpdate() {}

void Grid::render(const renderer::RendererContext &r) {
    for (int i = 1; i < 10; i++) {
        float x = i * 100;
        float y = x + 100;
        renderer::Renderer::drawLine(
            Vector2(x, 0), Vector2(x, GameData::WindowHeight), m_color);
        renderer::Renderer::drawLine(
            Vector2(0, y), Vector2(GameData::WindowWidth, y), m_color);
    }
}