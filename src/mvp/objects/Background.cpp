#include "Background.hpp"
#include "Renderer.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Background::Background(const Color &color) : scene::GameObject(Vector3(0)), m_color(color) {}

Background::~Background() {}

void Background::onStart() {

}

void Background::onUpdate() {

}   

void Background::render() const {
    const float height = 1200;
    const float width = 1000;
    renderer::Renderer::drawRectangle(Vector3(0), Vector2(width, height), m_color);
    for (int i = 1; i < 10; i++) {
        float x = i * 100;
        float y = x + 100;
        renderer::Renderer::drawLine(Vector2(x, 0), Vector2(x, height), Color::BLACK);
        renderer::Renderer::drawLine(Vector2(0, y), Vector2(width, y), Color::BLACK);
    }
}