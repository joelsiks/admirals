#pragma once

#include "IDrawable.hpp"
#include "IEventHandler.hpp"

namespace admirals {

class InteractiveDrawable : public renderer::IDrawable, public IEventHandler {
public:
    InteractiveDrawable(){};
    ~InteractiveDrawable(){};

    virtual void render(const renderer::RendererContext &r) const = 0;
    virtual void handleEvent(SDL_Event &e) = 0;
};

} // namespace admirals