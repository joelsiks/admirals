#pragma once
#include "DataObjects.hpp"

namespace admirals {

class NavMesh {
public:
    NavMesh() {}
    NavMesh(size_t width, size_t height, float *costs, float detailLevel,
            const Rect &bounds)
        : m_width(width), m_height(height), m_costs(costs),
          m_detailLevel(detailLevel), m_bounds(bounds) {}

    ~NavMesh() { delete[] m_costs; }

    inline Rect GetBounds() const { return m_bounds; }
    inline size_t GetGridWidth() const { return m_width; }
    inline size_t GetGridHeight() const { return m_height; }
    inline float GetLevelOfDetail() const { return m_detailLevel; }
    inline float GetCostAt(size_t index) const { return m_costs[index]; }
    inline const float *GetCost() const { return m_costs; }

protected:
    Rect m_bounds;
    size_t m_width = 0;
    size_t m_height = 0;
    float m_detailLevel = 1;

private:
    // Points to a 1 dimensional representation of a 2d grid of size m_width *
    // m_height, row by row.
    // A cost < 0 -> impassible
    float *m_costs = nullptr;
};

} // namespace admirals