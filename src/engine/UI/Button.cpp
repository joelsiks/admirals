
#include <iostream>

#include <VK2D/Renderer.h>
#include <VK2D/VK2d.h>

#include "Button.hpp"
#include "Util.hpp"

admirals::UI::Button::Button(const std::string& name, const std::string& text, vec2 size) : Element(name, text, size) {}

void admirals::UI::Button::Render(const VK2DTexture& font, const vec2& startPos) {
	// TODO: Use Element colors here (m_bgColor, m_fgColor).
	// Draw background.
	vk2dRendererSetColourMod(VK2D_BLACK);
    vk2dRendererDrawRectangle(startPos[0], startPos[1], m_displaySize[0], m_displaySize[1], 0, 0, 0);

	// Draw foreground (text).
	vk2dRendererSetColourMod(VK2D_WHITE);
	RenderFont(font, startPos[0], startPos[1], m_text.c_str());
	vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

bool admirals::UI::Button::HandleEvent(const SDL_Event& event) {
    return false;
}