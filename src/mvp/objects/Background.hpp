#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Background : public scene::GameObject {
public:
    Background(const Color &color);
    ~Background();

    void onUpdate() override;
    void onStart() override;
    void render() const override;

private:
    Color m_color;
};

} // namespace admirals::mvp::objects