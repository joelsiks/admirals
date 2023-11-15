#include <VK2D/VK2D.h>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text,
               const vec2 size, const vec4 backgroundColor,
               const vec4 foregroundColor)
    : Element(name, text, size) {
    for (int i = 0; i < 4; i++) {
        m_bgColor[i] = backgroundColor[i];
        m_fgColor[i] = foregroundColor[i];
    }
}

void Button::Render(const VK2DTexture font, const vec2 startPos) {
    renderer::Renderer::drawRectangle(startPos, m_displaySize, m_bgColor);
    renderer::Renderer::drawText(font, startPos, m_fgColor, m_text.c_str());
}

bool Button::HandleEvent(const SDL_Event &event) { return false; }