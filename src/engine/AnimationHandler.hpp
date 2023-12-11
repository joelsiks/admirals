#include "EngineContext.hpp"


class AnimationHandler
{
private:
    const float m_timeReset = 0.3f;

    //must be a factor of 2 in order to divide frames equally
    const int m_animationFrames = 8;

    const float

    int m_animationIndex = 0;
    float m_animationTimer = 0;

    const Vector2 m_emptySprite = Vector2(0, 0);
    std::vector<Vector2> textureMapBase;
    std::vector<Vector2> textureMapCruiser;
    std::vector<Vector2> textureMapDestroyer;
public:
    AnimationHandler();
    ~AnimationHandler();

    void SetupAnimationFrames();
    void UpdateAnimation(const EngineContext &ctx);
};

AnimationHandler::AnimationHandler(){}
AnimationHandler::~AnimationHandler(){}

void AnimationHandler::SetupAnimationFrames() {
    const float spriteSize = GameData::SpriteSize;

    
    auto offset = GameData::SpriteSize * 2;
    if (GameData::PlayerId != owner) {
        offset += GameData::SpriteSize * 3;
    }

    //setup cruiser
        textureMapCruiser.push_back(Vector2(offset + spriteSize * 2, 0));
        textureMapCruiser.push_back(
            Vector2(offset + spriteSize * 2, spriteSize));
        textureMapCruiser.push_back(Vector2(offset + spriteSize * 2,
                                        spriteSize * 2));
        textureMapCruiser.push_back(Vector2(offset + spriteSize * 2,
                                        spriteSize * 3));
    //setup destroyer
        textureMapDestroyer.push_back(Vector2(offset + spriteSize, 0));
        textureMapDestroyer.push_back(
            Vector2(offset + spriteSize, spriteSize));
        textureMapDestroyer.push_back(
            Vector2(offset + spriteSize, spriteSize * 2));
        textureMapDestroyer.push_back(
            Vector2(offset + spriteSize, spriteSize * 3));
    //setup base
        textureMapBase.push_back(Vector2(offset, spriteSize));
        textureMapBase.push_back(Vector2(offset, spriteSize * 2));
        textureMapBase.push_back(Vector2(offset, spriteSize * 3));
        textureMapBase.push_back(Vector2(offset, spriteSize * 4));
}

void AnimationHandler::UpdateAnimationTimer(const EngineContext &ctx) {
    m_animationTimer += ctx.deltaTime;
    if (m_animationTimer > m_timeReset) {
        m_animationTimer = 0;
        m_animationIndex += 1;
        m_animationIndex = m_animationIndex % m_animationFrames;
    }
}

void AnimationHandler::Render(const EngineContext &) const {
    renderer::Renderer::DrawSprite(m_source, GetPosition(),
                                   m_texOffset[m_texIndex], m_texSize,
                                   GetSize() / m_texSize);
}

