#include "UI/Data.hpp"

using namespace admirals;

Vector2 admirals::UI::GetPositionFromOrientation(DisplayOrientation orientation,
                                                 const Vector2 &elementSize,
                                                 const EngineContext &ctx) {
    Vector2 position(0, 0);

    // Handle center.
    if (orientation == DisplayOrientation::Center) {
        position[0] = (ctx.displayPort.Width() - elementSize[0]) / 2.0f;
        return position + ctx.displayPort.Position();
    }

    // Fix right offset.
    if (orientation == DisplayOrientation::UpperRight ||
        orientation == DisplayOrientation::LowerRight) {
        position[0] = ctx.displayPort.Width() - elementSize[0];
    }

    // Fix lower offset.
    if (orientation == DisplayOrientation::LowerLeft ||
        orientation == DisplayOrientation::LowerRight) {
        position[1] = ctx.displayPort.Height() - elementSize[1];
    }

    return position + ctx.displayPort.Position();
}