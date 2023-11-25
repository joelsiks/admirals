#pragma once

#include <memory>
#include <string>

#include <SDL_events.h>

#include "DataObjects.hpp"
#include "IDisplayable.hpp"
#include "IOrdered.hpp"
#include "InteractiveDrawable.hpp"

namespace admirals::UI {

enum DisplayOrientation {
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

    virtual DisplayOrientation GetDisplayOrientation() const {
        return m_dispOrient;
    }

    virtual void SetDisplayOrientation(DisplayOrientation pos) {
        m_dispOrient = pos;
    }

    template <typename T>
    static std::shared_ptr<Element> CreateFromDerived(const T &derivedObject) {
        // Assuming T is derived from Element
        std::shared_ptr<Element> element = std::make_shared<T>(derivedObject);
        return element;
    }

protected:
    // Text to be displayed on the Element.
    std::string m_text;

    DisplayOrientation m_dispOrient = DisplayOrientation::UpperLeft;
};

} // namespace admirals::UI