#include "Element.hpp"

using namespace admirals::UI;

Element::Element(const std::string &name, float order, const std::string &text,
                 const Vector2 &size)
    : m_name(name), m_order(order), m_text(text), m_displaySize(size) {}