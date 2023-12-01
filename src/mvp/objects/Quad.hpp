#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Quad : public GameObject {
public:
    Quad(const std::string &name, float order = 0, const Rect &bounds = Rect(),
         const Color &color = Color::WHITE);
    void Render(const EngineContext &ctx) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects