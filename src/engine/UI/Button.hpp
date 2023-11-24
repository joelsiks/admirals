#pragma once

#include <functional>

#include "Element.hpp"
#include "events/ButtonClickEvent.hpp"
#include "events/EventSystem.hpp"

namespace admirals::UI {

class Button : public Element {
public:
    events::EventSystem<events::ButtonClickEventArgs> onClick;

    Button(const std::string &name, float order, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor);

    virtual void Render(const renderer::RendererContext &r) const override;

    void OnClick(const events::MouseClickEventArgs &args) override;

    inline void SetBackgroundColor(const Color &color) { m_bgColor = color; }
    inline void SetForegroundColor(const Color &color) { m_fgColor = color; }

protected:
    Color m_bgColor, m_fgColor;
};

} // namespace admirals::UI