#pragma once

#include "DataObjects.hpp"
#include "EngineContext.hpp"

namespace admirals::UI {

enum class DisplayOrientation {
    UpperLeft = 0,
    UpperRight = 1,
    LowerLeft = 2,
    LowerRight = 3,
    Center = 4,
};

Vector2 GetPositionFromOrientation(DisplayOrientation orientation,
                                   const Vector2 &displaySize,
                                   const EngineContext &ctx);

} // namespace admirals::UI
