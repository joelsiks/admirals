#include "Animator.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

int Animator::frameInd = 0;

Animator::Animator(const std::string &name, const Texture &atlas)
    : GameObject(name), m_source(atlas) {}

Animator::~Animator() {}

void Animator::OnUpdate(const EngineContext &ctx) {
    m_aniTimer += ctx.deltaTime;
    if (m_aniTimer > m_timeReset) {
        m_aniTimer = 0;
        frameInd += 1;
        frameInd = frameInd % m_animationFrames;
    }
}

Vector2
Animator::GetAnimationFrame(const std::vector<Vector2> &animationVector) {
    int vectorLenght = animationVector.size();
    return animationVector[frameInd % vectorLenght];
}