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
    struct PF_Node {
        bool visited;
        float g, h, f;
        size_t path;
    };

    /// @description returns the heuristic value between the start and end node.
    /// The heuristic uses the manhattan distance equation
    /// @param start The start node
    /// @param end The end node
    /// @return The heuristic value from start to end
    inline static float Heuristic(const Vector2 &start, const Vector2 &end) {
        return std::min(std::abs(start.x() - end.x()),
                        std::abs(start.y() - end.y()));
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
    IsValidNextPosition(const Vector2 &nextPosition, const QuadTree &quadTree,
                        const std::unordered_set<float> &checkedOrders) {
        for (const auto &object : quadTree.GetObjectsAtPosition(nextPosition)) {
            const auto &o = static_pointer_cast<scene::GameObject>(object);
            if (o->GetBoundingBox().Contains(nextPosition) &&
                checkedOrders.contains(o->order())) {
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
    static std::deque<Vector2> FindPathRoot(const std::vector<PF_Node> &grid,
                                            size_t nodeIndex, size_t width,
                                            float detailLevel);

    /// @brief Finds the nodes neighboring the given position
    /// @param position The coordinates to find neighbors around
    /// @return An array of all neighboring nodes and null values (there
    ///         should always be at least 2 neighboring nodes)
    static std::vector<size_t>
    FindNeighboringNodes(size_t nodeIndex, size_t width, size_t height);

public:
    std::deque<Vector2> static FindPath(
        const QuadTree &quadTree, const Vector2 &start, const Vector2 &dest,
        const std::unordered_set<float> &checkedOrders, float detailLevel = 1);
};

} // namespace admirals