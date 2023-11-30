#pragma once
#include "objects/GridObject.hpp"

namespace admirals::mvp::objects {

class Sprite : public GridObject {
public:
    Sprite(const std::string &name, const Texture &source, float order = 0,
           const Rect &bounds = Rect(), const Vector2 &texOffset = Vector2(),
           const Vector2 &texSize = Vector2(GameData::SpriteSize));

    void Render(const EngineContext &ctx) const override;

private:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;
};

} // namespace admirals::mvp::objects