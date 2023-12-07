#pragma once

#include "IDisplayable.hpp"
#include "IOrdered.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals {

class IInteractiveDisplayable : public IOrdered, public IDisplayable {
public:
    IInteractiveDisplayable(const std::string &identifier, float order,
                            const Rect &bounds)
        : IOrdered(identifier, order), IDisplayable(bounds) {}

    virtual void OnClick(events::MouseClickEventArgs &args) = 0;

    virtual void OnMouseEnter(events::MouseMotionEventArgs &args) = 0;
    virtual void OnMouseLeave(events::MouseMotionEventArgs &args) = 0;
    virtual void OnMouseMove(events::MouseMotionEventArgs &args) = 0;

    virtual void OnShown() = 0;
    virtual void OnHidden() = 0;
};

} // namespace admirals