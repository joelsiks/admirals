#pragma once
#include "Renderer.hpp"
#include "UI/Button.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class IconifiedButton : public admirals::UI::Button {
public:
    IconifiedButton(const std::string &name, float order,
                    const std::string &text, const Vector2 &size,
                    const Color &bgColor, const Color &fgColor,
                    const Texture &source, const Vector2 &texOffset,
                    const Vector2 &texSize = Vector2(GameData::SpriteSize))
        : UI::Button(name, order, text, size, bgColor, fgColor),
          m_source(source), m_texOffset(texOffset), m_texSize(texSize) {}

    void Render(const EngineContext &ctx) const override {
        const Color &renderColor = m_shouldFadeBackground
                                       ? m_bgColor * Vector4(1, 1, 1, 0.75)
                                       : m_bgColor;

        renderer::Renderer::DrawRectangle(m_boundingBox, renderColor);

        renderer::Renderer::DrawRectangleOutline(m_boundingBox, 1, m_fgColor);

        renderer::Renderer::DrawSprite(m_source, m_boundingBox.Position(),
                                       m_texOffset, m_texSize,
                                       m_boundingBox.Size() / m_texSize);

        renderer::Renderer::DrawText(*ctx.fontTexture, m_boundingBox.Position(),
                                     m_fgColor, m_text, 2.f);
    }

private:
    const Texture &m_source;
    const Vector2 m_texSize;
    const Vector2 m_texOffset;
};
} // namespace admirals::mvp::objects