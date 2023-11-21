#pragma once

#include <functional>

#include "Element.hpp"
#include "events/EventSystem.hpp"

namespace admirals::UI {

class ButtonClickEventArgs : public events::EventArgs {
public:
    ButtonClickEventArgs(const SDL_Event &data) : m_data(data) {}
    const SDL_Event m_data;
};

class Button : public Element {
public:
    events::EventSystem<ButtonClickEventArgs> onClick;

    Button(const std::string &name, float order, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor);

    void Render(const Texture &font) override;

    bool HandleEvent(const SDL_Event &event) override;

    inline void SetBackgroundColor(const Color color) { m_bgColor = color; }
    inline void SetForegroundColor(const Color color) { m_fgColor = color; }

private:
    Color m_bgColor, m_fgColor;
};

} // namespace admirals::UI