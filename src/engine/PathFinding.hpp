#pragma once

#include <deque>
#include <unordered_set>
#include <vector>

#include "DataObjects.hpp"
#include "GameObject.hpp"
#include "QuadTree.hpp"

namespace admirals {

class PathFinding {
private:
    struct Node {
        bool visited = false;
        float g = 0;
        float h = 0;
        float f = 0;
        size_t path = -1;
    };

    struct Comparator {
        Comparator(const Node *grid) : m_grid(grid) {}

        inline bool operator()(size_t l, size_t r) const {
            return m_grid[l].f < m_grid[r].f;
        }

    private:
        const Node *m_grid;
    };

    /// @description returns the heuristic value between the start and end node.
    /// The heuristic uses the manhattan distance equation
    /// @param start The start node
    /// @param end The end node
    /// @return The heuristic value from start to end
    inline static float Heuristic(const Vector2 &start, const Vector2 &end) {
        return start.Distance(end);
    }

    /// @description returns the coordinates for a given vector value (index)
    /// @param position The position index
    /// @return Vector2 containing the x, y coordinates
    inline static Vector2
    ConvertNodeIndexToVector(size_t position, size_t width, float detailLevel) {
        return Vector2(
                   static_cast<float>(position % width),
                   static_cast<float>(static_cast<size_t>(position / width))) *
               detailLevel;
    }
    /// @description returns the vector value for the pathVector of a given
    /// position coordinate (index)
    /// @param position The Vector2 with the x, y coordinates to find neighbors
    /// around
    /// @return The position index
    inline static size_t ConvertVectorToNodeIndex(const Vector2 &position,
                                                  size_t width,
                                                  float detailLevel) {
        return static_cast<size_t>(position.x() / detailLevel) +
               static_cast<size_t>(position.y() / detailLevel) * width;
    }

    inline static bool
    IsValidPosition(const Vector2 &position, const Vector2 &pathSize,
                    const QuadTree &quadTree,
                    const std::unordered_set<float> &checkedOrders) {
        const Rect bounds = Rect(position, pathSize);
        for (const auto &object : quadTree.GetObjectsAtLocation(bounds)) {
            const auto &o = dynamic_pointer_cast<scene::GameObject>(object);
            if (checkedOrders.contains(o->order()) &&
                bounds.Overlaps(o->GetBoundingBox())) {
                return false;
            }
        }
        return true;
    }
    /// @description Finds the step the car should take (since traffic is always
    /// moving we do not need to chain the whole path as we need to do the
    /// algorithm from this new node)
    /// @param grid A vector containing all the nodes.
    /// @param nodeIndex the node that has the chain from the end position to
    /// the start position.
    /// @return The best node to travel to.
    static std::deque<Vector2> FindPathRoot(const Node *grid, size_t nodeIndex,
                                            size_t width, float detailLevel);

    /// @brief Finds the nodes neighboring the given position
    /// @param position The coordinates to find neighbors around
    /// @return An array of all neighboring nodes and null values (there
    ///         should always be at least 2 neighboring nodes)
    static void FindNeighboringNodes(size_t nodeIndex, size_t width,
                                     size_t height, size_t *data);

    static constexpr size_t NULLVALUE = static_cast<size_t>(-1);

public:
    static std::deque<Vector2>
    FindPath(const QuadTree &quadTree, const Vector2 &start,
             const Vector2 &dest, const Vector2 &pathSize,
             const std::unordered_set<float> &checkedOrders,
             float detailLevel = 1);
};

} // namespace admirals