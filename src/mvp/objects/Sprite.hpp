#pragma once
#include "GameObject.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class Sprite : public scene::GameObject {
public:
    Sprite(const std::string &name, const Texture &source, float order = 0,
           const Rect &bounds = Rect(), const Vector2 &texOffset = Vector2(),
           const Vector2 &texSize = Vector2(GameData::SpriteSize));
    void Render(const EngineContext &ctx) const override;

protected:
    virtual inline Vector2 CalcOrigin() const { return GetPosition(); }

private:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;
};

} // namespace admirals::mvp::objects