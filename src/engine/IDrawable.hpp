#pragma once
#include "Context.hpp"

namespace admirals::renderer {

struct RendererContext {
    int windowWidth;
    int windowHeight;

    // Indicates whether to draw outlines of elements for debugging purposes.
    bool renderDebugOutlines;
};

class IDrawable {
public:
    virtual void Render(const Context &c) const = 0;
};

} // namespace admirals::renderer