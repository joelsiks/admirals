
#pragma once

#include <functional>

#include "Element.hpp"

namespace admirals {
namespace UI {

class Button;

typedef std::function<void(Button *, const SDL_Event &)> ButtonOnClickHandler;

class Button : public Element {
public:
    Button(const std::string &name, float order, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor,
           ButtonOnClickHandler onClick);

    void Render(const Texture &font) override;

    bool HandleEvent(const SDL_Event &event) override;

    inline void SetBackgroundColor(const Color color) { m_bgColor = color; }
    inline void SetForegroundColor(const Color color) { m_fgColor = color; }

private:
    ButtonOnClickHandler m_onClick;
    Color m_bgColor, m_fgColor;
};

} // namespace UI
} // namespace admirals