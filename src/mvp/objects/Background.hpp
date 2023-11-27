#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Background : public scene::GameObject {
public:
    Background(const std::string &name, const Color &color);

    void OnStart(const EngineContext &ctx) override;
    void OnUpdate(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects