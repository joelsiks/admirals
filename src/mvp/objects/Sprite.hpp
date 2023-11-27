#pragma once
#include "GameObject.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class Sprite : public scene::GameObject {
public:
    Sprite(const std::string &name, const Vector3 &position,
           const Vector2 &size, const Texture &source, const Vector2 &texOffset,
           const Vector2 &texSize = Vector2(GameData::SpriteSize));
    Sprite(const std::string &name, float order, const Vector2 &position,
           const Vector2 &size, const Texture &source, const Vector2 &texOffset,
           const Vector2 &texSize = Vector2(GameData::SpriteSize));

    virtual void OnUpdate(const EngineContext &ctx) override;
    virtual void OnStart(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override;

protected:
    virtual inline Vector2 CalcOrigin() const { return GetPosition(); }

private:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;
    Vector2 m_size;
};

} // namespace admirals::mvp::objects