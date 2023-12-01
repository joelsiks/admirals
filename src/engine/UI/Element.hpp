#pragma once

#include <memory>
#include <string>

#include <SDL_events.h>

#include "DataObjects.hpp"
#include "IDisplayable.hpp"
#include "IInteractiveDisplayable.hpp"
#include "IOrdered.hpp"
#include "UI/Data.hpp"

namespace admirals::UI {

// General UI element that can be rendered.
class Element : public IInteractiveDisplayable {
public:
    Element(const std::string &name, float order, const std::string &text,
            const Vector2 &size);

    virtual void OnClick(events::MouseClickEventArgs &args) override {}

    virtual void OnMouseEnter(events::MouseMotionEventArgs &args) override {}
    virtual void OnMouseLeave(events::MouseMotionEventArgs &args) override {}
    virtual void OnMouseMove(events::MouseMotionEventArgs &args) override {}

    virtual void OnShown() override {}
    virtual void OnHidden() override {}

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