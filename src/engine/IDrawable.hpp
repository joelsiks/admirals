#pragma once
#include "EngineContext.hpp"

namespace admirals::renderer {

class IDrawable {
public:
    virtual void Render(const EngineContext &c) const = 0;
};

} // namespace admirals::renderer