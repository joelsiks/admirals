#pragma once

#include <functional>

#include "Element.hpp"
#include "events/EventSystem.hpp"

namespace admirals::UI {

class Button : public Element {
public:
    events::EventSystem<events::MouseClickEventArgs> onClick;

    Button(const std::string &name, float order, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor);

    virtual void Render(const EngineContext &ctx) const override;

    void OnClick(events::MouseClickEventArgs &args) override;

    virtual void OnMouseEnter(events::MouseMotionEventArgs &args) override;
    virtual void OnMouseLeave(events::MouseMotionEventArgs &args) override;
    virtual void OnMouseMove(events::MouseMotionEventArgs &args) override;

    virtual void OnHidden() override;

    inline void SetBackgroundColor(const Color &color) {
        m_bgColor = color;
        m_bgColorFaded = color * Vector4(1, 1, 1, 0.75);
    }

    inline void SetForegroundColor(const Color &color) { m_fgColor = color; }

protected:
    Color m_bgColor, m_bgColorFaded, m_fgColor;
    bool m_shouldFadeBackground = false;
};

} // namespace admirals::UI