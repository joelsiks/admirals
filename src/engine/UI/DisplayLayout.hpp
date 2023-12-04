#pragma once

#include <unordered_set>

#include "IDisplayLayer.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public IDisplayLayer {
public:
    void Render(const EngineContext &ctx) const override;
};

} // namespace admirals::UI