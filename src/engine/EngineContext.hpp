#pragma once
#include <chrono>

namespace admirals {

struct EngineContext {
    int windowWidth;
    int windowHeight;

    // Indicates whether to draw outlines of elements for debugging purposes.
    bool renderDebugOutlines;

    double deltaTime;
};

} // namespace admirals
