
#include <iostream>

#include <VK2D/VK2D.h>

#include "Renderer.hpp"
#include "TextElement.hpp"

using namespace admirals::UI;

TextElement::TextElement(const std::string &name, const std::string &text,
                         const Vector2 &size)
    : Element(name, text, size) {}

void TextElement::SetText(const std::string &text) { this->m_text = text; }

void TextElement::Render(const VK2DTexture font, const Vector2 &startPos) {
    // TODO: Use Element colors here (m_fgColor).
    // Draw foreground (text).
    renderer::Renderer::drawText(font, startPos, Color::BLACK,
                                 this->m_text.c_str());
}

bool TextElement::HandleEvent(const SDL_Event &event) { return false; }