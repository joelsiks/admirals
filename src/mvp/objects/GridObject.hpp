#pragma once
#include "GameObject.hpp"

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

    static float GetGridScale();

    static Vector2 GetGridOffset();

    static Vector2 ConvertPositionGridToWorld(const Vector2 &gridPosition);

    static Vector2 ConvertPositionWorldToGrid(const Vector2 &worldPosition);

protected:
    static float GetGridScale(const Vector2 &windowSize);

    static Vector2 GetGridOffset(const Vector2 &windowSize, float scale);
};
} // namespace admirals::mvp::objects