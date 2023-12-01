#include <set>

#include "PathFinding.hpp"

using namespace admirals;

std::deque<Vector2>
PathFinding::FindPath(const QuadTree &quadTree, const Vector2 &start,
                      const Vector2 &dest, const Vector2 &pathSize,
                      const std::unordered_set<float> &checkedOrders,
                      float detailLevel) {
    const Rect bounds = Rect(Vector2(), quadTree.GetSize());

    if (!bounds.Contains(start) || !bounds.Contains(dest) ||
        !IsValidPosition(start, Vector2(__FLT_EPSILON__), quadTree,
                         checkedOrders) ||
        !IsValidPosition(dest, Vector2(__FLT_EPSILON__), quadTree,
                         checkedOrders)) {
        return std::deque<Vector2>();
    }

    const size_t width = static_cast<size_t>((bounds.Width() / detailLevel));
    const size_t height = static_cast<size_t>((bounds.Height() / detailLevel));
    // world grid representing possible move locations
    Node *grid = new Node[width * height];

    const size_t startIndex =
        ConvertVectorToNodeIndex(start, width, detailLevel);
    const size_t endIndex = ConvertVectorToNodeIndex(dest, width, detailLevel);

    std::set<size_t, PathFinding::Comparator> orderedNodeIndexQueue =
        std::set<size_t, PathFinding::Comparator>(
            PathFinding::Comparator(grid));
    orderedNodeIndexQueue.insert(startIndex);
    grid[startIndex].visited = true;
    grid[startIndex].path = NULLVALUE;

    size_t neighbors[8];
    while (!orderedNodeIndexQueue.empty()) {
        const size_t nodeIndex = *orderedNodeIndexQueue.begin();
        orderedNodeIndexQueue.erase(orderedNodeIndexQueue.begin());

        if (nodeIndex == endIndex) {
            const auto path = FindPathRoot(grid, nodeIndex, width, detailLevel);
            delete[] grid;
            return path;
        }

        FindNeighboringNodes(nodeIndex, width, height, neighbors);
        const float g = grid[nodeIndex].g + detailLevel;
        for (const size_t neighborIndex : neighbors) {
            if (neighborIndex == NULLVALUE) {
                continue;
            }
            Node &neighbor = grid[neighborIndex];
            if (!neighbor.visited || neighbor.g > g) {
                const Vector2 position =
                    ConvertNodeIndexToVector(neighborIndex, width, detailLevel);
                neighbor.visited = true;
                if (IsValidPosition(position, pathSize, quadTree,
                                    checkedOrders)) {
                    neighbor.h = Heuristic(position, dest, pathSize);
                    neighbor.g = g;
                    neighbor.f = neighbor.h + g;
                    neighbor.path = nodeIndex;
                    orderedNodeIndexQueue.insert(neighborIndex);
                }
            }
        }
    }

    delete[] grid;
    return std::deque<Vector2>();
}

std::deque<Vector2> PathFinding::FindPathRoot(const Node *grid,
                                              size_t nodeIndex, size_t width,
                                              float detailLevel) {
    std::deque<Vector2> path = {};

    while (grid[nodeIndex].path != NULLVALUE) {
        path.push_front(
            ConvertNodeIndexToVector(nodeIndex, width, detailLevel));
        nodeIndex = grid[nodeIndex].path;
    }

    return path;
}

void PathFinding::FindNeighboringNodes(size_t nodeIndex, size_t width,
                                       size_t height, size_t *data) {
    const size_t x = nodeIndex % width;
    const size_t y = nodeIndex / width;

    if (x > 0) {
        data[0] = nodeIndex - 1;
    } else {
        data[0] = NULLVALUE;
    }
    if (x < width - 1) {
        data[1] = nodeIndex + 1;
    } else {
        data[1] = NULLVALUE;
    }
    if (y > 0) {
        data[2] = nodeIndex - width;
    } else {
        data[2] = NULLVALUE;
    }
    if (y < height - 1) {
        data[3] = nodeIndex + width;
    } else {
        data[3] = NULLVALUE;
    }
    // Diagonals
    if (x > 0 && y < height - 1) {
        data[4] = nodeIndex - 1 + width;
    } else {
        data[4] = NULLVALUE;
    }
    if (x < width - 1 && y < height - 1) {
        data[5] = nodeIndex + 1 + width;
    } else {
        data[5] = NULLVALUE;
    }
    if (x > 0 && y > 0) {
        data[6] = nodeIndex - 1 - width;
    } else {
        data[6] = NULLVALUE;
    }
    if (x < width - 1 && y > 0) {
        data[7] = nodeIndex + 1 - width;
    } else {
        data[7] = NULLVALUE;
    }
}