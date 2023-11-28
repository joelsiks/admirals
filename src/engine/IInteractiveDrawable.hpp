#pragma once

#include "IDrawable.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals {

class IInteractiveDrawable : public renderer::IDrawable {
public:
    virtual void OnClick(events::MouseClickEventArgs &args) = 0;
    virtual void OnMouseEnter(events::MouseMotionEventArgs &args) = 0;
    virtual void OnMouseLeave(events::MouseMotionEventArgs &args) = 0;
    virtual void OnMouseMove(events::MouseMotionEventArgs &args) = 0;
};

} // namespace admirals