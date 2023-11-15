
#include <iostream>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text,
               const Vector2 &size, const Color &bgColor, const Color &fgColor)
    : Element(name, text, size), m_bgColor(bgColor), m_fgColor(fgColor) {}

void Button::Render(const VK2DTexture font, const Vector2 &startPos) {
    renderer::Renderer::drawRectangle(startPos, m_displaySize, m_bgColor);
    renderer::Renderer::drawText(font, startPos, m_fgColor, m_text);
}

bool Button::HandleEvent(const SDL_Event &event) { return false; }