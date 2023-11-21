#include "Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;
using namespace admirals::events;

Button::Button(const std::string &name, float order, const std::string &text,
               const Vector2 &size, const Color &bgColor, const Color &fgColor)
    : Element(name, order, text, size), m_bgColor(bgColor), m_fgColor(fgColor) {
}

void Button::Render(const Texture &font) {
    renderer::Renderer::DrawRectangle(m_displayOrigin, m_displaySize,
                                      m_bgColor);
    renderer::Renderer::DrawText(font, m_displayOrigin, m_fgColor, m_text);
}

bool Button::HandleEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        const float mouseX = static_cast<float>(event.button.x);
        const float mouseY = static_cast<float>(event.button.y);

        if (mouseX >= m_displayOrigin[0] &&
            mouseX <= m_displayOrigin[0] + m_displaySize[0] &&
            mouseY >= m_displayOrigin[1] &&
            mouseY <= m_displayOrigin[1] + m_displaySize[1]) {

            // Call click handler.
            ButtonClickEventArgs e = ButtonClickEventArgs(event);
            onClick.Invoke(this, e);

            return true;
        }
    }

    return false;
}