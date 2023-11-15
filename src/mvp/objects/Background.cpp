#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const Color &color)
    : scene::GameObject(Vector3(0)), m_color(color) {}

Background::~Background() {}

void Background::onStart() {}

void Background::onUpdate() {}

void Background::render() const {
    renderer::Renderer::drawRectangle(
        Vector3(0), Vector2(GameData::WindowWidth, GameData::WindowHeight),
        m_color);
}