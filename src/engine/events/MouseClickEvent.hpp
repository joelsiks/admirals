#pragma once

#include <cstdint>

#include <SDL_events.h>
#include <SDL_mouse.h>

#include "DataObjects.hpp"
#include "events/EventArgs.hpp"

namespace admirals::events {

enum MouseButton : uint8_t {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2,
};

class MouseClickEventArgs : public EventArgs {
public:
    MouseClickEventArgs(const SDL_MouseButtonEvent &e)
        : button(static_cast<MouseButton>(e.button)), clicks(e.clicks),
          pressed(e.state == SDL_PRESSED), x(e.x), y(e.y) {
        SDL_GetWindowSize(SDL_GetWindowFromID(e.windowID), &windowWidth,
                          &windowHeight);
    }

    inline Vector2 Location() const {
        return Vector2(static_cast<float>(x), static_cast<float>(y));
    }

    inline Vector2 WindowSize() const {
        return Vector2(static_cast<float>(windowWidth),
                       static_cast<float>(windowHeight));
    }

    const MouseButton button; // what button is clicked
    const uint8_t clicks;     // the number of clicks
    const bool pressed;       // true = UP, false = DOWN
    const int32_t x;
    const int32_t y;
    int windowWidth;
    int windowHeight;
};

} // namespace admirals::events