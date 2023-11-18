#pragma once
#include "GameObject.hpp"

namespace admirals::mvp::objects {

class Sprite : public scene::GameObject {
public:
    Sprite(const std::string &name, const Vector3 position, const Vector2 &size,
           const Texture &source, const Vector2 &texOffset,
           const Vector2 &texSize = Vector2(64));

    virtual void OnUpdate() override;
    virtual void OnStart() override;
    void Render(const renderer::RendererContext &r) const override;

private:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;
    Vector2 m_size;
};

} // namespace admirals::mvp::objects