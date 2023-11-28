#pragma once

#include <cstdint>

#include <SDL_events.h>
#include <SDL_mouse.h>

namespace admirals::events {

enum class MouseButton : uint8_t {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2,
};

}