
#pragma once

#include <SDL_rect.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class Button : public Element {
public:
    Button(const std::string &name, const std::string &text, const vec2 size,
           const vec4 backgroundColor, const vec4 foregroundColor);

    virtual void Render(const VK2DTexture font, const vec2 startPos);

    virtual bool HandleEvent(const SDL_Event &event) override;

private:
    vec4 m_bgColor, m_fgColor;
};

} // namespace UI
} // namespace admirals