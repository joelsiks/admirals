#pragma once

#include <vector>

#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Animator : public GameObject {
public:
    Animator(const std::string &name);
    ~Animator();

    void OnStart(const EngineContext &ctx) override{};
    void OnUpdate(const EngineContext &ctx);
    void Render(const EngineContext &ctx) const override {}

    Vector2 GetAnimationFrame(const std::vector<Vector2> &animationVector);

private:
    const float m_timeReset = 0.3f;

    // must be a factor of 2 in order to divide frames equally
    const int m_totalFrames = 8;
    double m_frameTimer = 0;
    int m_frameIndex;
};

} // namespace admirals::mvp::objects