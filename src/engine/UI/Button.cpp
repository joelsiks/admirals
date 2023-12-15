#include "UI/Button.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;
using namespace admirals::events;

Button::Button(const std::string &name, float order, const std::string &text,
               const Vector2 &size, const Color &bgColor, const Color &fgColor)
    : Element(name, order, text, size), m_fgColor(fgColor) {
    SetBackgroundColor(bgColor);
}

void Button::Render(const EngineContext &ctx) const {
    const Color &renderColor =
        m_shouldFadeBackground ? m_bgColorFaded : m_bgColor;

    renderer::Renderer::DrawRectangle(m_boundingBox, renderColor);
    renderer::Renderer::DrawText(*ctx.fontTexture, m_boundingBox.Position(),
                                 m_fgColor, m_text, 2.f);
}

void Button::OnClick(events::MouseClickEventArgs &args) {
    onClick.Invoke(this, args);
}

void Button::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_shouldFadeBackground = true;
    renderer::Renderer::SetCursor(renderer::Cursor::Hand);
}

void Button::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_shouldFadeBackground = false;
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);
}

void Button::OnMouseMove(events::MouseMotionEventArgs &) {
    m_shouldFadeBackground = true;
    renderer::Renderer::SetCursor(renderer::Cursor::Hand);
}

void Button::OnHidden() {
    m_shouldFadeBackground = false;
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);
}