#pragma once

#include "DataObjects.hpp"
#include "events/EventArgs.hpp"
#include "events/MouseEvents.hpp"

namespace admirals::events {

class MouseCLickEventArgs : public EventArgs {
public:
    MouseCLickEventArgs(const SDL_MouseButtonEvent &e)
        : button(static_cast<MouseButton>(e.button)), clicks(e.clicks),
          pressed(e.state == SDL_PRESSED), x(e.x), y(e.y) {
        SDL_GetWindowSize(SDL_GetWindowFromID(e.windowID), &m_windowWidth,
                          &m_windowHeight);
    }

    inline Vector2 Location() const {
        return Vector2(static_cast<float>(x), static_cast<float>(y));
    }

    inline Vector2 WindowSize() const {
        return Vector2(static_cast<float>(m_windowWidth),
                       static_cast<float>(m_windowHeight));
    }

    const MouseButton button; // what button is clicked
    const uint8_t clicks;     // the number of clicks
    const bool pressed;
    const int32_t x;
    const int32_t y;

private:
    int m_windowWidth;
    int m_windowHeight;
};

} // namespace admirals::events