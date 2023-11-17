#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Grid : public scene::GameObject {
public:
    Grid(const Color &color);
    ~Grid();

    void onUpdate() override;
    void onStart() override;
    void render(const renderer::RendererContext &r) override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects