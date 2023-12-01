#include "UI/Data.hpp"

using namespace admirals;
using namespace admirals::UI;

Vector2 GetPositionFromOrientation(DisplayOrientation orientation,
                                   const Vector2 &displaySize,
                                   const EngineContext &ctx) {
    Vector2 position(0, 0);

    // Handle center.
    if (orientation == DisplayOrientation::Center) {
        position[0] = (ctx.windowSize.x() - displaySize[0]) / 2.0f;
        return position;
    }

    // Fix right offset.
    if (orientation == DisplayOrientation::UpperRight ||
        orientation == DisplayOrientation::LowerRight) {
        position[0] = ctx.windowSize.x() - displaySize[0];
    }

    // Fix lower offset.
    if (orientation == DisplayOrientation::LowerLeft ||
        orientation == DisplayOrientation::LowerRight) {
        position[1] = ctx.windowSize.y() - displaySize[1];
    }

    return position;
}