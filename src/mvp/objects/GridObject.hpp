#pragma once
#include "GameObject.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class GridObject : public GameObject {
    using GameObject::GameObject; // Inherit constructors

public:
    virtual inline Rect GetBoundingBox() const override {
        return Rect(GetPosition(), GetSize());
    };

    virtual inline Vector2 GetPosition() const override {
        return ConvertPositionGridToWorld(m_boundingBox.Position());
    };

    virtual inline Vector2 GetSize() const override {
        return m_boundingBox.Size() * GetGridScale();
    };

    static inline float GetGridScale() {
        const Vector2 windowSize = GameData::engine->GetWindowSize();
        return GetGridScale(windowSize);
    }

    static inline Vector2 GetGridOffset() {
        const Vector2 windowSize = GameData::engine->GetWindowSize();
        const float scale = GetGridScale(windowSize);
        return GetGridOffset(windowSize, scale);
    }

    static inline Vector2
    ConvertPositionGridToWorld(const Vector2 &gridPosition) {
        const Vector2 windowSize = GameData::engine->GetWindowSize();
        const float scale = GetGridScale(windowSize);
        const Vector2 offset = GetGridOffset(windowSize, scale);
        return offset + (GameData::GridArea.Position() +
                         gridPosition * GameData::CellSize) *
                            scale;
    }

    static inline Vector2
    ConvertPositionWorldToGrid(const Vector2 &worldPosition) {
        const Vector2 windowSize = GameData::engine->GetWindowSize();
        const float scale = GetGridScale(windowSize);
        const Vector2 offset = GetGridOffset(windowSize, scale);
        const Vector2 res = (((worldPosition - offset) / scale) -
                             GameData::GridArea.Position()) /
                            GameData::CellSize;
        return res;
    }

private:
    static inline float GetGridScale(const Vector2 &windowSize) {
        const float dim =
            std::min(windowSize.x() + GameData::CellSize * 2, windowSize.y());
        return std::max<float>(0, dim) / GameData::TotalHeight;
    }

    static inline Vector2 GetGridOffset(const Vector2 &windowSize,
                                        float scale) {
        return (windowSize -
                Vector2(GameData::GridSize, GameData::TotalHeight) * scale) /
               2.f;
    }
};
} // namespace admirals::mvp::objects