#include <VK2D/VK2D.h>

#include "Renderer.hpp"
#include "TextElement.hpp"

using namespace admirals::UI;

TextElement::TextElement(const std::string &name, const std::string &text,
                         const vec2 size, const vec4 color)
    : Element(name, text, size) {

    for (int i = 0; i < 4; i++) {
        m_textColor[i] = color[i];
    }
}

void TextElement::SetText(const std::string &text) { this->m_text = text; }

void TextElement::Render(const VK2DTexture font) {
    renderer::Renderer::drawText(font, m_displayOrigin, m_textColor,
                                 this->m_text.c_str());
}

bool TextElement::HandleEvent(const SDL_Event &event) { return false; }