#pragma once

#include "DataObjects.hpp"

namespace admirals {

struct EngineContext {
    Vector2 windowSize;

    // Represents thepartof the window that is used for displaying the game.
    Rect displayPort;

    // Indicates whether to draw outlines of elements for debugging purposes.
    uint32_t debug;

    double deltaTime;

    Texture *fontTexture;
    Vector2 fontSize;
};

} // namespace admirals
