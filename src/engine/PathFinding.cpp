#include <set>

#include "PathFinding.hpp"

using namespace admirals;

std::deque<Vector2> PathFinding::FindPath(const Vector2 &start,
                                          const Vector2 &dest,
                                          const NavMesh &navMesh,
                                          bool validateStart,
                                          bool validateDest) {
    const Rect bounds = navMesh.GetBounds();
    if (!bounds.Contains(start) || !bounds.Contains(dest)) {
        return std::deque<Vector2>();
    }

    // Data from nav-mesh
    const size_t width = navMesh.GetGridWidth();
    const size_t height = navMesh.GetGridHeight();
    const float detailLevel = navMesh.GetLevelOfDetail();

    // start + end indices
    const size_t startIndex =
        ConvertVectorToNodeIndex(start - bounds.Position(), width, detailLevel);
    const size_t endIndex =
        ConvertVectorToNodeIndex(dest - bounds.Position(), width, detailLevel);
    const Vector2 endLocation =
        ConvertNodeIndexToVector(endIndex, width, detailLevel) +
        detailLevel / 2.f;

    if ((validateStart && navMesh.GetCostAt(startIndex) < 0) ||
        (validateDest && navMesh.GetCostAt(endIndex) < 0) ||
        startIndex == endIndex) {
        return std::deque<Vector2>();
    }

    // world grid representing possible move locations
    Node *grid = new Node[width * height];

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

        FindNeighboringNodes(nodeIndex, width, height, neighbors);
        for (const size_t neighborIndex : neighbors) {
            if (neighborIndex == NULLVALUE) {
                continue;
            }

            Node &neighbor = grid[neighborIndex];

            if (neighborIndex == endIndex) {
                neighbor.path = nodeIndex;
                const auto path = FindPathRoot(grid, neighborIndex, width,
                                               detailLevel, bounds.Position());
                delete[] grid;
                return path;
            }

            if (!neighbor.visited || neighbor.g > grid[nodeIndex].g) {
                const Vector2 position =
                    ConvertNodeIndexToVector(neighborIndex, width, detailLevel);
                neighbor.visited = true;
                const float cost = navMesh.GetCostAt(neighborIndex);
                if (cost >= 0) {
                    neighbor.h = Heuristic(position, endLocation, detailLevel);
                    neighbor.g = grid[nodeIndex].g + cost;
                    neighbor.f = neighbor.h + neighbor.g;
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
                                              float detailLevel,
                                              const Vector2 &offset) {
    std::deque<Vector2> path = {};

    while (grid[nodeIndex].path != NULLVALUE) {
        path.push_front(
            ConvertNodeIndexToVector(nodeIndex, width, detailLevel) + offset);
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