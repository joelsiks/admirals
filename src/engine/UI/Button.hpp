
#pragma once

#include <SDL_rect.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class Button : public Element {
public:
    Button(const std::string &name, const std::string &text,
           const Vector2 &size, const Color &bgColor, const Color &fgColor);

    virtual void Render(const VK2DTexture font, const Vector2 &startPos);

    virtual bool HandleEvent(const SDL_Event &event) override;

private:
    Color m_bgColor, m_fgColor;
};

} // namespace UI
} // namespace admirals