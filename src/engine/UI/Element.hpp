
#pragma once

#include <memory>
#include <string>

#include <SDL_events.h>
#include <VK2D/Structs.h>

namespace admirals {
namespace UI {

enum DisplayPosition {
    UpperLeft = 0,
    UpperRight = 1,
    LowerLeft = 2,
    LowerRight = 3,
};

// General UI element that can be rendered.
class Element {
public:
    Element(const std::string &name, const std::string &text, vec2 size);
    virtual ~Element(){};

    // Getters and setters
    virtual const std::string &GetName() const { return m_name; }
    virtual void SetName(const std::string &name) { m_name = name; }

    virtual const vec2 &GetDisplaySize() const { return m_displaySize; }
    virtual void SetDisplaySize(const vec2 size) {
        m_displaySize[0] = size[0];
        m_displaySize[1] = size[1];
    }

    virtual DisplayPosition GetDisplayPosition() const { return m_displayPos; }
    virtual void SetDisplayPosition(DisplayPosition pos) { m_displayPos = pos; }

    // Pure virtual function for rendering the UI element
    virtual void Render(const VK2DTexture font, const vec2 startPos) = 0;

    // Input handling
    virtual bool HandleEvent(const SDL_Event &event) { return false; }

    template <typename T>
    static std::shared_ptr<Element> createFromDerived(const T &derivedObject) {
        // Assuming T is derived from Element
        std::shared_ptr<Element> element = std::make_shared<T>(derivedObject);
        return element;
    }

protected:
    // The name of the Element.
    std::string m_name;

    // Text to be displayed on the Element.
    std::string m_text;

    // Colors for rendering.
    vec4 m_fgColor, m_bgColor;

    // Size for displaying/rendering the element to the screen.
    vec2 m_displaySize;

    DisplayPosition m_displayPos = DisplayPosition::UpperLeft;
};

} // namespace UI
} // namespace admirals