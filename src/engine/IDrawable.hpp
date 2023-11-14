#pragma once

namespace admirals {
namespace renderer {
class IDrawable {
public:
    IDrawable(){};
    ~IDrawable(){};
    virtual void render() const = 0;
};
} // namespace renderer
} // namespace admirals