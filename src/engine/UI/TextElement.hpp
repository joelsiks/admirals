
#pragma once

#include <SDL_rect.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class TextElement : public Element {
public:
    TextElement(const std::string &name, const std::string &text,
                const Vector2 &size);

    void SetText(const std::string &text);

    void Render(const VK2DTexture font, const Vector2 &startPos);

    bool HandleEvent(const SDL_Event &event) override;
};

} // namespace UI
} // namespace admirals