#pragma once

#include <memory>
#include <string>

#include <SDL_events.h>

#include "DataObjects.hpp"
#include "IDisplayable.hpp"
#include "IOrdered.hpp"
#include "InteractiveDrawable.hpp"

namespace admirals::UI {

enum DisplayPosition {
    UpperLeft = 0,
    UpperRight = 1,
    LowerLeft = 2,
    LowerRight = 3,
    Center = 4,
};

// General UI element that can be rendered.
class Element : public IOrdered,
                public InteractiveDrawable,
                public IDisplayable {
public:
    Element(const std::string &name, float order, const std::string &text,
            const Vector2 &size);

    // Getters and setters
    virtual Vector2 GetDisplaySize() const { return m_boundingBox.Size(); }
    virtual void SetDisplaySize(const Vector2 &size) {
        m_boundingBox.SetSize(size);
    }

    virtual Vector2 GetDisplayOrigin() const {
        return m_boundingBox.Position();
    }
    virtual void SetDisplayOrigin(const Vector2 &origin) {
        m_boundingBox.SetPosition(origin);
    }

    virtual DisplayPosition GetDisplayPosition() const { return m_displayPos; }
    virtual void SetDisplayPosition(DisplayPosition pos) { m_displayPos = pos; }

    template <typename T>
    static std::shared_ptr<Element> CreateFromDerived(const T &derivedObject) {
        // Assuming T is derived from Element
        std::shared_ptr<Element> element = std::make_shared<T>(derivedObject);
        return element;
    }

protected:
    // Text to be displayed on the Element.
    std::string m_text;

    DisplayPosition m_displayPos = DisplayPosition::UpperLeft;
};

} // namespace admirals::UI