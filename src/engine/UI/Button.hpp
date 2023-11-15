
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
    Button(const std::string &name, const std::string &text, const vec2 size, const vec4 backgroundColor, const vec4 foregroundColor,
           ButtonOnClickHandler onClick);

    virtual void Render(const VK2DTexture font);

    virtual bool HandleEvent(const SDL_Event &event) override;

    void SetBackgroundColor(const vec4 color);

private:
    ButtonOnClickHandler m_onClick;
    vec4 m_bgColor, m_fgColor;
};

} // namespace UI
} // namespace admirals