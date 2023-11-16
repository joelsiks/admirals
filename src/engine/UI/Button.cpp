#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

Button::Button(const std::string &name, float order, const std::string &text,
               const Vector2 &size, const Color &bgColor, const Color &fgColor,
               ButtonOnClickHandler onClick)
    : Element(name, order, text, size), m_bgColor(bgColor), m_fgColor(fgColor),
      m_onClick(onClick) {}

void Button::Render(const Texture &font) {
    renderer::Renderer::drawRectangle(m_displayOrigin, m_displaySize,
                                      m_bgColor);
    renderer::Renderer::drawText(font, m_displayOrigin, m_fgColor, m_text);
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