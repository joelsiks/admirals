
#pragma once

#include <functional>

#include <SDL_rect.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class Button;

typedef std::function<void(Button *, const SDL_Event &)> ButtonOnClickHandler;

class Button : public Element {
public:
    Button(const std::string &name, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor,
           ButtonOnClickHandler onClick);

    virtual void Render(const VK2DTexture font);

    virtual bool HandleEvent(const SDL_Event &event) override;

    void SetBackgroundColor(const vec4 color);

private:
    ButtonOnClickHandler m_onClick;
    Color m_bgColor, m_fgColor;
};

} // namespace UI
} // namespace admirals