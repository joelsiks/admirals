#pragma once

#include "DataObjects.hpp"

namespace admirals {

class IDisplayable {
public:
    IDisplayable() : m_boundingBox(Rect()) {}
    IDisplayable(const Rect &bounds) : m_boundingBox(bounds) {}

    inline Rect GetBoundingBox() const { return m_boundingBox; };

protected:
    Rect m_boundingBox;
};

} // namespace admirals
