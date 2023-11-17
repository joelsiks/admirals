#pragma once

#include "IDrawable.hpp"
#include "IEventHandler.hpp"

namespace admirals {

class InteractiveDrawable : public renderer::IDrawable, public IEventHandler {
public:
    InteractiveDrawable(){};
    ~InteractiveDrawable(){};

    virtual void Render(const renderer::RendererContext &r) const = 0;

    virtual void HandleEvent(SDL_Event &e) = 0;
};

} // namespace admirals