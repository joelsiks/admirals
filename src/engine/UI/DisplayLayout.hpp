#pragma once

#include <memory>
#include <vector>

#include "InteractiveDrawable.hpp"
#include "OrderedCollection.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public InteractiveDrawable {
public:
    DisplayLayout();

    void Render(const EngineContext &c) const override;
    void HandleEvent(SDL_Event &e) override;

    void AddElement(std::shared_ptr<Element> element);

    static Vector2 GetOriginFromDisplayPosition(DisplayPosition pos,
                                                const Vector2 &displaySize,
                                                const EngineContext &c);

    inline Vector2 TextFontSize(const std::string &text) const {
        return Vector2(static_cast<float>(text.length()) * m_fontWidth,
                       m_fontHeight);
    }

protected:
    Texture m_font;
    float m_fontWidth, m_fontHeight;

    OrderedCollection<Element> m_elements;
};

} // namespace admirals::UI