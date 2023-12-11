#pragma once

#include "Base.hpp"
#include "CommonTypes.hpp"
#include "GameData.hpp"
#include "Renderer.hpp"
#include "TreasureIsland.hpp"
#include <vector>

namespace admirals::mvp::objects {

enum SpriteType {
    ShipType,
    Island,
    none,
};

class Animator : public GameObject {
public:
    Animator(const std::string &name, const Texture &m_atlas);
    ~Animator();

    void OnStart(const EngineContext &ctx) override{};
    void OnUpdate(const EngineContext &ctx);
    void Render(const EngineContext &ctx) const override {}

    static Vector2
    GetAnimationFrame(const std::vector<Vector2> &animationVector);

    static void RenderAnimationFrame(const Sprite &object);

    static int frameInd;

private:
    const float m_timeReset = 0.3f;

    // must be a factor of 2 in order to divide frames equally
    const int m_animationFrames = 8;
    float m_aniTimer = 0;

    const Texture &m_source;
    static constexpr Vector2 m_texSize = Vector2(GameData::SpriteSize);

    const Vector2 m_emptySprite = Vector2(0, 0);
    // std::map<ShipType, std::vector<Vector2>> m_neutralTexture;
    // std::map<ShipType, std::vector<Vector2>> m_playerTexture;
    // std::map<ShipType, std::vector<Vector2>> m_enemyTexture;
};

} // namespace admirals::mvp::objects
