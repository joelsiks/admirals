
#include <VK2D/Renderer.h>
#include <VK2D/VK2D.h>

#include "Element.hpp"

using namespace admirals::UI;

Element::Element(const std::string &name, const std::string &text, vec2 size) {
    this->m_name = name;
    this->m_text = text;
    this->m_displaySize[0] = size[0];
    this->m_displaySize[1] = size[1];
    vk2dColourHex(this->m_fgColor, "FFFFFF");
    vk2dColourHex(this->m_bgColor, "000000");
}