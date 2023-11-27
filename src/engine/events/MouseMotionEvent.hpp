#pragma once

#include "DataObjects.hpp"
#include "events/EventArgs.hpp"
#include "events/MouseEvents.hpp"

namespace admirals::events {

class MouseMotionEventArgs : public EventArgs {
public:
    MouseMotionEventArgs(const SDL_MouseMotionEvent &e)
        : x(e.x), y(e.y), dx(e.xrel), dy(e.yrel), state(e.state) {
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

    inline Vector2 Motion() const {
        return Vector2(static_cast<float>(dx), static_cast<float>(dy));
    }

    inline Vector2 Direction() const {
        const Vector2 movement = Motion();
        return movement / movement.Magnitude();
    }

    inline bool IsButtonDown(MouseButton button) const {
        switch (button) {
        case MouseButton::Left:
            return state & SDL_BUTTON_LMASK != 0;
        case MouseButton::Middle:
            return state & SDL_BUTTON_MMASK != 0;
        case MouseButton::Right:
            return state & SDL_BUTTON_RMASK != 0;
        case MouseButton::X1:
            return state & SDL_BUTTON_X1MASK != 0;
        case MouseButton::X2:
            return state & SDL_BUTTON_X2MASK != 0;
        default:
            return false;
        }
    }

    inline float Velocity() const { return Motion().Magnitude(); }

    const int32_t x;
    const int32_t y;
    const int32_t dx;
    const int32_t dy;
    const uint32_t state; // 32-bit button bitmask of the current button state

private:
    int m_windowWidth;
    int m_windowHeight;
};

} // namespace admirals::events