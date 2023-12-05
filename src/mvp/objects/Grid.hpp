#pragma once
#include "objects/GridObject.hpp"

namespace admirals::mvp::objects {

class Grid : public GridObject {
public:
    Grid(const std::string &name, const Color &color);

    void Render(const EngineContext &ctx) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects