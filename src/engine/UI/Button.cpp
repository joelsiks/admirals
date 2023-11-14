#include <VK2D/VK2D.h>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text, vec2 size)
    : Element(name, text, size) {}

void Button::Render(const VK2DTexture font, const vec2 startPos) {
    // TODO: Use Element colors here (m_bgColor, m_fgColor).
    renderer::Renderer::drawRectangle(startPos, m_displaySize, VK2D_BLACK);
    renderer::Renderer::drawText(font, startPos, VK2D_WHITE, m_text.c_str());
}

bool Button::HandleEvent(const SDL_Event &event) { return false; }