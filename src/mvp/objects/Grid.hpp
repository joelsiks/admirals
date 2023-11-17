#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Grid : public scene::GameObject {
public:
    Grid(const std::string &name, const Color &color);
    ~Grid();

    void OnStart() override;
    void OnUpdate() override;
    void Render(const renderer::RendererContext &r) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects