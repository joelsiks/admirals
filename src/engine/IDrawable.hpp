#pragma once

namespace admirals::renderer {

struct RendererContext {
    int windowWidth;
    int windowHeight;

    // Indicates whether to draw outlines of elements for debugging purposes.
    bool renderDebugOutlines;
};

class IDrawable {
public:
    virtual void Render(const RendererContext &r) const = 0;
};

} // namespace admirals::renderer