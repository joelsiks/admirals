#include "Grid.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Grid::Grid(const std::string &name, const Color &color)
    : scene::GameObject(name, 1000, Vector2(0, 0)), m_color(color) {}

Grid::~Grid() {}

void Grid::OnStart() {}

void Grid::OnUpdate() {}

void Grid::Render(const renderer::RendererContext &r) const {
    for (int i = 1; i < 10; i++) {
        float x = i * 100;
        float y = x + 100;
        renderer::Renderer::DrawLine(
            Vector2(x, 0), Vector2(x, r.windowHeight), m_color);
        renderer::Renderer::DrawLine(
            Vector2(0, y), Vector2(r.windowWidth, y), m_color);
    }
}