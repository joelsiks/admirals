
#pragma once

#include <SDL_rect.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class TextElement : public Element {
public:
    TextElement(const std::string& name, const std::string& text, vec2 size);

    virtual void Render(const VK2DTexture& font, const vec2& startPos);

    virtual bool HandleEvent(const SDL_Event& event) override;
};

};
};