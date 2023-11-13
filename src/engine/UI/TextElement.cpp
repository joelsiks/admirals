
#include <iostream>

#include <VK2D/VK2D.h>

#include "TextElement.hpp"

admirals::UI::TextElement::TextElement(const std::string& name, const std::string& text, vec2 size) : Element(name, text, size) {}

void admirals::UI::TextElement::Render(const VK2DTexture& font, const vec2& startPos) {
	// TODO: Use Element colors here (m_fgColor).
	// Draw foreground (text).
	vk2dRendererSetColourMod(VK2D_WHITE);
	RenderFont(font, startPos[0], startPos[1], m_text.c_str());
	vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

bool admirals::UI::TextElement::HandleEvent(const SDL_Event& event) {
    return false;
}