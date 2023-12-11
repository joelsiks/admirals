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

/*
void Animator::RenderAnimationFrame(const Sprite &object) {
    int vectorLenght = object.m_texOffset.size();
    // GetTypeOfSprite(object);
    renderer::Renderer::DrawSprite(object.m_source, object.GetPosition(),
                                   object.m_texOffset[frameInd % vectorLenght],
                                   object.m_texSize,
                                   object.GetSize() / object.m_texSize);
}*/

/*
void Animator::OnStart(const EngineContext &) {
    float spriteSize = 64.;
    // GameData::SpriteSize;
    std::vector<Vector2> textureMapCruiser;
    std::vector<Vector2> textureMapEnemyCruiser;
    std::vector<Vector2> textureMapDestroyer;
    std::vector<Vector2> textureMapEnemyDestroyer;
    std::vector<Vector2> textureMapBase;
    std::vector<Vector2> textureMapEnemyBase;
    std::vector<Vector2> textureMapIsland;
    // setup cruiser
    textureMapCruiser.push_back(Vector2(spriteSize * 4, 0));
    textureMapCruiser.push_back(Vector2(spriteSize * 4, spriteSize));
    textureMapCruiser.push_back(Vector2(spriteSize * 4, spriteSize * 2));
    textureMapCruiser.push_back(Vector2(spriteSize * 4, spriteSize * 3));
    // setup enemy cruiser
    textureMapEnemyCruiser.push_back(Vector2(spriteSize * 7, 0));
    textureMapEnemyCruiser.push_back(Vector2(spriteSize * 7, spriteSize));
    textureMapEnemyCruiser.push_back(Vector2(spriteSize * 7, spriteSize * 2));
    textureMapEnemyCruiser.push_back(Vector2(spriteSize * 7, spriteSize * 3));
    // setup destroyer
    textureMapDestroyer.push_back(Vector2(spriteSize * 3, 0));
    textureMapDestroyer.push_back(Vector2(spriteSize * 3, spriteSize));
    textureMapDestroyer.push_back(Vector2(spriteSize * 3, spriteSize * 2));
    textureMapDestroyer.push_back(Vector2(spriteSize * 3, spriteSize * 3));
    // setup enemy destroyer
    textureMapEnemyDestroyer.push_back(Vector2(spriteSize * 6, 0));
    textureMapEnemyDestroyer.push_back(Vector2(spriteSize * 6, spriteSize));
    textureMapEnemyDestroyer.push_back(Vector2(spriteSize * 6, spriteSize * 2));
    textureMapEnemyDestroyer.push_back(Vector2(spriteSize * 6, spriteSize * 3));
    // setup base
    textureMapBase.push_back(Vector2(spriteSize * 2, spriteSize));
    textureMapBase.push_back(Vector2(spriteSize * 2, spriteSize * 2));
    textureMapBase.push_back(Vector2(spriteSize * 2, spriteSize * 3));
    textureMapBase.push_back(Vector2(spriteSize * 2, spriteSize * 4));
    // setup enemy base
    textureMapEnemyBase.push_back(Vector2(spriteSize * 5, spriteSize));
    textureMapEnemyBase.push_back(Vector2(spriteSize * 5, spriteSize * 2));
    textureMapEnemyBase.push_back(Vector2(spriteSize * 5, spriteSize * 3));
    textureMapEnemyBase.push_back(Vector2(spriteSize * 5, spriteSize * 4));
    // setup island base
    textureMapIsland.push_back(Vector2(spriteSize * 1, spriteSize * 2));

    m_neutralTexture.emplace(Island, textureMapIsland);

    m_playerTexture.emplace(Ship_Cruiser, textureMapCruiser);
    m_playerTexture.emplace(Ship_Destroyer, textureMapDestroyer);
    m_playerTexture.emplace(Ship_Base, textureMapBase);

    m_enemyTexture.emplace(Ship_Cruiser, textureMapEnemyCruiser);
    m_enemyTexture.emplace(Ship_Destroyer, textureMapEnemyDestroyer);
    m_enemyTexture.emplace(Ship_Base, textureMapEnemyBase);
}*/