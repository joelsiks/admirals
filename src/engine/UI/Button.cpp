
#include <iostream>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text, Vector2 size)
    : Element(name, text, size) {}

void Button::Render(const VK2DTexture font, const Vector2 &startPos) {
    // TODO: Use Element colors here (m_bgColor, m_fgColor).
    renderer::Renderer::drawRectangle(startPos, m_displaySize, Color::BLACK);
    renderer::Renderer::drawText(font, startPos, Color::WHITE, m_text);
}

bool Button::HandleEvent(const SDL_Event &event) { return false; }