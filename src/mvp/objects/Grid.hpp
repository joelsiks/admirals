#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Grid : public scene::GameObject {
public:
    Grid(const std::string &name, const Color &color);

    void OnStart(Context &c) override;
    void OnUpdate(Context &c) override;
    void Render(const Context &c) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects