#include "UI/Element.hpp"

using namespace admirals::UI;

Element::Element(const std::string &name, float order, const std::string &text,
                 const Vector2 &size)
    : IOrdered(name, order), m_text(text),
      IDisplayable(Rect(0, 0, size.x(), size.y())) {}