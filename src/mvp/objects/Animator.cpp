

#include "Animator.hpp"
#include "GameData.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Animator::Animator(const std::string &name) : GameObject(name) {}
Animator::~Animator() {}

void Animator::OnUpdate(const EngineContext &ctx) {
    m_frameTimer += ctx.deltaTime;
    if (m_frameTimer > m_timeReset) {
        m_frameTimer = 0;
        m_frameIndex += 1;
        m_frameIndex = m_frameIndex % m_totalFrames;
    }
}

Vector2
Animator::GetAnimationFrame(const std::vector<Vector2> &animationVector) {
    return animationVector[m_frameIndex % animationVector.size()];
}