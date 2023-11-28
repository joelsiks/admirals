#pragma once

#include "DataObjects.hpp"

namespace admirals {

class IDisplayable {
public:
    IDisplayable() {}
    IDisplayable(const Rect &bounds) : m_boundingBox(bounds) {}

    virtual inline bool IsVisible() const {
        return m_boundingBox.Width() > 0 && m_boundingBox.Height() > 0;
    }

    virtual inline Rect GetBoundingBox() const { return m_boundingBox; };
    // virtual inline Rect &GetBoundingBox() { return m_boundingBox; };

    virtual inline Vector2 GetPosition() const {
        return m_boundingBox.Position();
    };
    virtual inline void SetPosition(const Vector2 &newPosition) {
        m_boundingBox.SetPosition(newPosition);
    }
    virtual inline void SetPosition(float x, float y) {
        m_boundingBox.SetPositionX(x);
        m_boundingBox.SetPositionY(y);
    }

    virtual inline Vector2 GetSize() const { return m_boundingBox.Size(); };
    virtual inline void SetSize(const Vector2 &newSize) {
        m_boundingBox.SetSize(newSize);
    }
    virtual inline void SetSize(float width, float height) {
        m_boundingBox.SetWidth(width);
        m_boundingBox.SetHeight(height);
    }

protected:
    Rect m_boundingBox;
};

} // namespace admirals
