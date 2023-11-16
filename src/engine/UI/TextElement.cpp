#include "TextElement.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

TextElement::TextElement(const std::string &name, const std::string &text,
                         const Vector2 &size, const Color &color)
    : Element(name, text, size), m_textColor(color) {}

void TextElement::SetText(const std::string &text) { this->m_text = text; }

void TextElement::Render(const Texture &font) {
    renderer::Renderer::drawText(font, m_displayOrigin, m_textColor,
                                 this->m_text.c_str());
}

bool TextElement::HandleEvent(const SDL_Event &event) { return false; }