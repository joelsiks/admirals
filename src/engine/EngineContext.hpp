#pragma once
#include "DataObjects.hpp"

namespace admirals {

struct EngineContext {
    Vector2 windowSize;

    // Indicates whether to draw outlines of elements for debugging purposes.
    bool renderDebugOutlines;

    double deltaTime;
};

} // namespace admirals
