#include "UI/Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;
using namespace admirals::events;

Button::Button(const std::string &name, float order, const std::string &text,
               const Vector2 &size, const Color &bgColor, const Color &fgColor)
    : Element(name, order, text, size), m_bgColor(bgColor), m_fgColor(fgColor) {
}

void Button::Render(const EngineContext &c) const {
    renderer::Renderer::DrawRectangle(m_boundingBox, m_bgColor);
    renderer::Renderer::DrawText(*c.fontTexture, m_boundingBox.Position(),
                                 m_fgColor, m_text);
}

void Button::OnClick(events::MouseClickEventArgs &args) {
    onClick.Invoke(this, args);
}