#pragma once

#include <deque>
#include <unordered_set>
#include <vector>

#include "DataObjects.hpp"
#include "NavMesh.hpp"

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
    /// @param detailLevel The size of each individual step in the path.
    /// @return The heuristic value from start to end
    inline static float Heuristic(const Vector2 &start, const Vector2 &end,
                                  float detailLevel) {
        return start.Distance(end - detailLevel / 2.f);
    }

    /// @brief Finds the path from the root node to the given nodeIndex.
    /// @param grid The world grid containing all the nodes.
    /// @param nodeIndex The index of the node to find the path to
    /// @param width The width of the world grid (number of nodes).
    /// @param detailLevel The size of each individual step in the path.
    /// @param offset The position offset of each step in the path.
    /// @return The path from the root node to the node with the given
    /// nodeIndex.
    static std::deque<Vector2> FindPathRoot(const Node *grid, size_t nodeIndex,
                                            size_t width, float detailLevel,
                                            const Vector2 &offset);

    /// @brief Finds the nodes neighboring the given position
    /// @param position The coordinates to find neighbors around
    /// @return An array of all neighboring nodes and null values (there
    ///         should always be at least 2 neighboring nodes)
    static void FindNeighboringNodes(size_t nodeIndex, size_t width,
                                     size_t height, size_t *data);

    static constexpr size_t NULLVALUE = static_cast<size_t>(-1);

public:
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

    /// @brief Finds the path from the start position to the destination using
    /// the A* algorithm.
    /// @param start The starting position of the path.
    /// @param dest The destination position of the path.
    /// @param navMesh A navigation-mesh to find the path on.
    /// @param validateStart If the start must be valid to get a path
    /// @param validateDest If the destination must be valid to get a path
    /// @return A queue containing the steps to take from the start to
    /// destination, or an empty queue if no valid path exists.
    static std::deque<Vector2>
    FindPath(const Vector2 &start, const Vector2 &dest, const NavMesh &navMesh,
             bool validateStart = false, bool validateDest = true);
};

} // namespace admirals