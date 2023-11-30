#pragma once
#include "GameObject.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {
const float TotalHeight = GameData::GridSize + GameData::CellSize * 2;

class GridObject : public scene::GameObject {
    using scene::GameObject::GameObject; // Inherit constructors

public:
    virtual void OnUpdate(const EngineContext &ctx) override {
        const float dim = std::min(ctx.windowSize.x() + GameData::CellSize * 2,
                                   ctx.windowSize.y());
        m_scale = std::max<float>(0, dim) / TotalHeight;
        m_offset = (ctx.windowSize -
                    Vector2(GameData::GridSize, TotalHeight) * m_scale) /
                   2.f;
    }

    inline Rect GetBoundingBox() const override {
        return Rect(GetPosition(), GetSize());
    };
    virtual inline Vector2 GetPosition() const override {
        return m_offset + (GameData::GridArea.Position() +
                           m_boundingBox.Position() * GameData::CellSize) *
                              m_scale;
    };
    virtual inline Vector2 GetSize() const override {
        return m_boundingBox.Size() * m_scale;
    };

protected:
    Vector2 m_offset;
    float m_scale; // TODO
};
} // namespace admirals::mvp::objects