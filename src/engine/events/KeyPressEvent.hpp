#pragma once

#include <cstdint>

#include <SDL_events.h>
#include <SDL_keyboard.h>

#include "events/EventArgs.hpp"

namespace admirals::events {

class KeyPressEventArgs : public EventArgs {
public:
    KeyPressEventArgs(const SDL_KeyboardEvent &e)
        : key(e.keysym.sym), isKeyUp(e.state == SDL_PRESSED) {}

    const SDL_Keycode key; // The key that is pressed
    const bool isKeyUp;
};

} // namespace admirals::events