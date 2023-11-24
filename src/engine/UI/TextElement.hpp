#pragma once

#include "UI/Element.hpp"

namespace admirals::UI {

class TextElement : public Element {
public:
    TextElement(const std::string &name, float order, const std::string &text,
                const Vector2 &size, const Color &color);

    void SetText(const std::string &text);

    void Render(const renderer::RendererContext &r) const override;

    virtual void OnClick(const events::MouseClickEventArgs &args) override;

private:
    Color m_textColor;
};

} // namespace admirals::UI