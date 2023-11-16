#pragma once

namespace admirals {
namespace renderer {
class Renderer;

class IDrawable {
public:
    IDrawable(){};
    ~IDrawable(){};

    virtual void render(const renderer::Renderer *r) const = 0;
};

} // namespace renderer
} // namespace admirals