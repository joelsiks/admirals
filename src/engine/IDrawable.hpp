#pragma once
#include "EngineContext.hpp"

#include "DataObjects.hpp"

namespace admirals::renderer {

class IDrawable {
public:
    virtual void Render(const EngineContext &ctx) const = 0;
};

} // namespace admirals::renderer