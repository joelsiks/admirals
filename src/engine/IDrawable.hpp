#pragma once

namespace admirals {
namespace renderer {

struct RendererContext {
    int windowWidth;
    int windowHeight;
};

class IDrawable {
public:
    IDrawable(){};
    ~IDrawable(){};

    virtual void render(const RendererContext &r) const = 0;
};

} // namespace renderer
} // namespace admirals