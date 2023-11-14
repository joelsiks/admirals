
#pragma once

namespace admirals {
namespace UI {

enum DisplayPosition {
    UpperLeft = 0,
    UpperRight = 1,
    LowerLeft = 2,
    LowerRight = 3,
};

// Very basic and simple font renderer
void RenderFont(const VK2DTexture &font, float x, float y, const char *text);

}; // namespace UI
}; // namespace admirals