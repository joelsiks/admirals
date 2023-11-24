#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Grid : public scene::GameObject {
public:
    Grid(const std::string &name, const Color &color);

    void OnStart(const EngineContext &c) override;
    void OnUpdate(const EngineContext &c) override;
    void Render(const EngineContext &c) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects