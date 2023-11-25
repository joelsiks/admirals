#pragma once

#include "DataObjects.hpp"

namespace admirals {

class IDisplayable {
public:
    IDisplayable() {}
    IDisplayable(const Rect &bounds) : m_boundingBox(bounds) {}

    inline Rect GetBoundingBox() const { return m_boundingBox; };

    inline Vector2 GetDisplayPosition() const {
        return m_boundingBox.Position();
    };

    inline void SetDisplayPosition(const Vector2 &newPosition) {
        m_boundingBox.SetPosition(newPosition);
    }

    inline Vector2 GetDisplaySize() const { return m_boundingBox.Size(); };

    inline void SetDisplaySize(const Vector2 &newSize) {
        m_boundingBox.SetSize(newSize);
    }

protected:
    Rect m_boundingBox;
};

} // namespace admirals
