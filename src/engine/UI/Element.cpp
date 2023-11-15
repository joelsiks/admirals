#include <VK2D/Renderer.h>

#include "Element.hpp"

using namespace admirals::UI;

Element::Element(const std::string &name, const std::string &text,
                 const vec2 size) {
    this->m_name = name;
    this->m_text = text;
    this->m_displaySize[0] = size[0];
    this->m_displaySize[1] = size[1];
}