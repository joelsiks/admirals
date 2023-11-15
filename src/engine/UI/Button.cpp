#include <VK2D/VK2D.h>

#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, const std::string &text, vec2 size,
               ButtonOnClickHandler onClick)
    : Element(name, text, size), m_onClick(onClick) {}

void Button::Render(const VK2DTexture font) {
    // TODO: Use Element colors here (m_bgColor, m_fgColor).
    renderer::Renderer::drawRectangle(m_displayOrigin, m_displaySize,
                                      VK2D_BLACK);
    renderer::Renderer::drawText(font, m_displayOrigin, VK2D_WHITE,
                                 m_text.c_str());
}

bool Button::HandleEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        // Check if the mouse click event occurred within the button's area
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Assuming buttonRect is the SDL_Rect representing the button's area
        if (mouseX >= m_displayOrigin[0] &&
            mouseX <= m_displayOrigin[0] + m_displaySize[0] &&
            mouseY >= m_displayOrigin[1] &&
            mouseY <= m_displayOrigin[1] + m_displaySize[1]) {
            // Mouse click happened within the button's area
            SetBackgroundColor(VK2D_GREEN);
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