#pragma once
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <cstdint>

namespace admirals::events {

enum MouseButton : uint8_t {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2,
};

class MouseCLickEventArgs : public events::EventArgs {
public:
    MouseCLickEventArgs(const SDL_MouseButtonEvent &e)
        : button(static_cast<MouseButton>(e.button)), clicks(e.clicks),
          pressed(e.state == SDL_PRESSED), x(e.x), y(e.y) {
        SDL_GetWindowSize(SDL_GetWindowFromID(e.windowID), &windowWidth,
                          &windowHeight);
    }

    inline Vector2 location() const {
        return Vector2(static_cast<float>(x), static_cast<float>(y));
    }

    const MouseButton button; // what button is clicked
    const uint8_t clicks;     // the number of clicks
    const bool pressed;
    const int32_t x;
    const int32_t y;
    int windowWidth;
    int windowHeight;
};

} // namespace admirals::events