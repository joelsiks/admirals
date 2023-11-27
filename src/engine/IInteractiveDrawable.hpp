#pragma once

#include "IDrawable.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class IInteractiveDrawable : public renderer::IDrawable {
public:
    virtual void OnClick(events::MouseClickEventArgs &args) = 0;
    virtual void OnMouseEnter(events::EventArgs &args) = 0;
    virtual void OnMouseLeave(events::EventArgs &args) = 0;
    virtual void OnMouseMove(events::EventArgs &args) = 0;
};

} // namespace admirals