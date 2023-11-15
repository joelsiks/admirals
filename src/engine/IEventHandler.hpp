#pragma once

#include <SDL_events.h>

namespace admirals {

class IEventHandler {
public:
    IEventHandler(){};
    ~IEventHandler(){};

    virtual void handleEvent(SDL_Event &e) = 0;
};

} // namespace admirals