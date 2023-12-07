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
        float g = 0; // the cost from the start node, to this node
        float h = 0; // estimated cost from the destination node, to this node
        float f = 0; // combined cost of g and h
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

    /// @brief returns the heuristic value between the start and end node.
    /// The heuristic uses the euclidean distance equation
    /// @param start The start node
    /// @param end The end node
    /// @return The heuristic value from start to end
    inline static float Heuristic(const Vector2 &start, const Vector2 &end,
                                  const Vector2 &pathSize) {
        return start.Distance(end - pathSize / 2.f);
    }

    /// @brief returns the coordinates for a given node index
    /// @param index The node index to convert
    /// @param width The width of the world grid (number of nodes)
    /// @param detailLevel The size of each individual step in the path.
    /// @return Vector2 containing the x, y coordinates
    inline static Vector2 ConvertNodeIndexToVector(size_t index, size_t width,
                                                   float detailLevel) {
        return Vector2(static_cast<float>(index % width),
                       static_cast<float>(static_cast<size_t>(index / width))) *
               detailLevel;
    }
    /// @brief returns the closest node index for the given position
    /// @param position The position to convert
    /// @param width The width of the world grid (number of nodes)
    /// @param detailLevel The size of each individual step in the path.
    /// @return The position index
    inline static size_t ConvertVectorToNodeIndex(const Vector2 &position,
                                                  size_t width,
                                                  float detailLevel) {
        return static_cast<size_t>(position.x() / detailLevel) +
               static_cast<size_t>(position.y() / detailLevel) * width;
    }

    /// @brief Checks if any objects in the quadtree overlap with the path at a
    /// given position
    /// @param position The position to check
    /// @param pathSize The size of the path
    /// @param quadTree The quadtree containing objects to check
    /// @param checkedOrders The object orders to check
    /// @return true if there are no overlapping objects with any of the given
    /// orders, false otherwise.
    inline static bool
    IsValidPosition(const Vector2 &position, const Vector2 &pathSize,
                    const QuadTree &quadTree,
                    const std::unordered_set<float> &checkedOrders) {
        const Rect bounds = Rect(position, pathSize);
        for (const auto &object : quadTree.GetObjectsInArea(bounds)) {
            const auto &o = dynamic_pointer_cast<GameObject>(object);
            if (checkedOrders.contains(o->order()) &&
                bounds.Overlaps(o->GetBoundingBox())) {
                return false;
            }
        }
        return true;
    }

    /// @brief Finds the path from the root node to the given nodeIndex.
    /// @param grid The world grid containing all the nodes.
    /// @param nodeIndex The index of the node to find the path to
    /// @param width The width of the world grid (number of nodes).
    /// @param detailLevel The size of each individual step in the path.
    /// @return The path from the root node to the node with the given
    /// nodeIndex.
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
    /// @brief Finds the path from the start position to the destination using
    /// the A* algorithm.
    /// @param quadTree A quadtree containing the obstacles the path may avoid.
    /// @param start The starting position of the path.
    /// @param dest The destination position of the path.
    /// @param pathSize The width and height of the path, used when checking
    /// collisions.
    /// @param checkedOrders The orders for objects in the quadtree which we
    /// want to avoid.
    /// @param detailLevel The size of each individual step in the path.
    /// @return A queue containing the steps to take from the root node to the
    /// node, or an empty queue if no valid path exists.
    static std::deque<Vector2>
    FindPath(const QuadTree &quadTree, const Vector2 &start,
             const Vector2 &dest, const Vector2 &pathSize,
             const std::unordered_set<float> &checkedOrders,
             float detailLevel = 1);
};

} // namespace admirals