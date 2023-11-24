#include "UI/TextElement.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

TextElement::TextElement(const std::string &name, float order,
                         const std::string &text, const Vector2 &size,
                         const Color &color)
    : Element(name, order, text, size), m_textColor(color) {}

void TextElement::SetText(const std::string &text) { m_text = text; }

void TextElement::Render(const renderer::RendererContext &r) const {
    renderer::Renderer::DrawText(*r.fontTexture, m_boundingBox.Position(),
                                 m_textColor, m_text);
}

void TextElement::OnClick(const events::MouseClickEventArgs &args) {}