
#pragma once

#include "Element.hpp"

namespace admirals::UI {

class TextElement : public Element {
public:
    TextElement(const std::string &name, const std::string &text,
                const Vector2 &size, const Color &color);

    void SetText(const std::string &text);

    void Render(const Texture &font) override;

    bool HandleEvent(const SDL_Event &event) override;

private:
    Color m_textColor;
};

} // namespace admirals::UI