#include <VK2D/VK2D.h>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text,
               const vec2 size, const vec4 backgroundColor,
               const vec4 foregroundColor, ButtonOnClickHandler onClick)
    : Element(name, text, size), m_onClick(onClick) {
    for (int i = 0; i < 4; i++) {
        m_bgColor[i] = backgroundColor[i];
        m_fgColor[i] = foregroundColor[i];
    }
}

void Button::Render(const VK2DTexture font) {
    renderer::Renderer::drawRectangle(m_displayOrigin, m_displaySize,
                                      m_bgColor);
    renderer::Renderer::drawText(font, m_displayOrigin, m_fgColor,
                                 m_text.c_str());
}

bool Button::HandleEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (mouseX >= m_displayOrigin[0] &&
            mouseX <= m_displayOrigin[0] + m_displaySize[0] &&
            mouseY >= m_displayOrigin[1] &&
            mouseY <= m_displayOrigin[1] + m_displaySize[1]) {

            // Call click handler.
            m_onClick(this, event);

            return true;
        }
    }

    return false;
}

void Button::SetBackgroundColor(const vec4 color) {
    for (int i = 0; i < 4; i++) {
        m_bgColor[i] = color[i];
    }
}