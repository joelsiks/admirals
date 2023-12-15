#pragma once
#include "objects/GridObject.hpp"

namespace admirals::mvp::objects {

class Background : public GridObject {
public:
    Background(const std::string &name, const Color &color,
               const Color &outerColor);

    void Render(const EngineContext &ctx) const override;

private:
    Color m_color;
    Color m_outerColor;
};

} // namespace admirals::mvp::objects