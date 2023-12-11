#pragma once

#include <unordered_set>

#include "IDisplayLayer.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public IDisplayLayer {
public:
    virtual void Render(const EngineContext &ctx) const override;
    virtual void Update(const EngineContext &ctx) override;
};

} // namespace admirals::UI