#pragma once
#include "GameData.hpp"
#include "objects/GridObject.hpp"

namespace admirals::mvp::objects {

class Sprite : public GridObject {
public:
    Sprite(const std::string &name, const Texture &source, float order = 0,
           const Rect &bounds = Rect(), const Vector2 &texOffset = Vector2(0),
           const Vector2 &texSize = Vector2(GameData::SpriteSize));

    void Render(const EngineContext &ctx) const override;

protected:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;

    void DrawSprite(const Rect &bounds) const;
};

} // namespace admirals::mvp::objects