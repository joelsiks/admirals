#pragma once

#include "IDrawable.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class InteractiveDrawable : public renderer::IDrawable {
public:
    InteractiveDrawable(){};
    ~InteractiveDrawable(){};

    virtual void Render(const EngineContext &ctx) const = 0;

    virtual void OnClick(events::MouseClickEventArgs &args) = 0;
};

} // namespace admirals