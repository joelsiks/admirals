#pragma once

namespace admirals::renderer {

class IDrawable {
public:
    IDrawable(){};
    ~IDrawable(){};

    virtual void render() const = 0;
};

} // namespace admirals::renderer