#include "PathFinding.hpp"

using namespace admirals;

const std::vector<Vector2> ADJACENT_DIRECTIONS = {
    {1, 0}, {-1, 0},  {0, 1},  {0, -1}, // vertical and horizontal directions
    {1, 1}, {-1, -1}, {1, -1}, {-1, 1}  // diagonal directions
};

std::deque<Vector2> PathFinding::FindPath(
    const QuadTree &quadTree, const Vector2 &start, const Vector2 &dest,
    const std::unordered_set<float> &checkedOrders, float detailLevel) {
    const Rect bounds = Rect(Vector2(), quadTree.GetSize());
    if (!bounds.Contains(start) || !bounds.Contains(dest)) {
        return std::deque<Vector2>();
    }

    const size_t width = static_cast<size_t>(bounds.Width() / detailLevel);
    const size_t height = static_cast<size_t>(bounds.Height() / detailLevel);
    // world grid
    std::vector<PF_Node> grid(width * height,
                              {false, 0, 0, 0, static_cast<size_t>(-1)});
    const size_t startIndex =
        ConvertVectorToNodeIndex(start, width, detailLevel);
    const size_t endIndex = ConvertVectorToNodeIndex(dest, width, detailLevel);

    std::vector<size_t> queue = {startIndex};
    grid[startIndex].visited = true;
    size_t minIndex = 0;
    float minValue = INFINITY;

    while (!queue.empty()) {
        // 1. Find nodes that can be passed (neighboring passed nodes)
        // 2. Calculate values of nodes not passed
        // 3. "pass" node of lowest value & set "path" value to the node it
        // traveled from
        // 4. If end node is passed, break.
        const size_t nodeIndex = queue[minIndex];
        queue.erase(queue.begin() + static_cast<int>(minIndex));

        if (nodeIndex == endIndex) {
            const auto path = FindPathRoot(grid, nodeIndex, width, detailLevel);
            return path;
        }

        const auto neighbors = FindNeighboringNodes(nodeIndex, width, height);
        for (const size_t neighborIndex : neighbors) {
            if (neighborIndex == static_cast<size_t>(-1)) {
                continue;
            }
            PF_Node &neighbor = grid[neighborIndex];
            if (!neighbor.visited) {
                const Vector2 position =
                    ConvertNodeIndexToVector(neighborIndex, width, detailLevel);
                neighbor.visited = true;
                if (IsValidNextPosition(position, quadTree, checkedOrders)) {
                    neighbor.h = Heuristic(position, dest);
                    neighbor.g = grid[nodeIndex].g + 1;
                    neighbor.f = neighbor.h + neighbor.g;
                    neighbor.path = nodeIndex;
                    queue.push_back(neighborIndex);
                }
            }
        }

        minValue = INFINITY;
        for (int index = 0; index < queue.size(); index++) {
            const size_t nodeIndex = queue[index];
            if (grid[nodeIndex].f < minValue) {
                minValue = grid[nodeIndex].f;
                minIndex = index;
            }
        }
    }

    return std::deque<Vector2>();
}

std::deque<Vector2> PathFinding::FindPathRoot(const std::vector<PF_Node> &grid,
                                              size_t nodeIndex, size_t width,
                                              float detailLevel) {
    std::deque<Vector2> path = {};

    while (grid[nodeIndex].path != static_cast<size_t>(-1)) {
        path.push_front(
            ConvertNodeIndexToVector(nodeIndex, width, detailLevel));
        nodeIndex = grid[nodeIndex].path;
    }

    return path;
}

std::vector<size_t> PathFinding::FindNeighboringNodes(size_t nodeIndex,
                                                      size_t width,
                                                      size_t height) {
    std::vector<size_t> neighbors;
    const size_t x = nodeIndex % width;
    const size_t y = nodeIndex / width;
    if (x > 0) {
        neighbors.push_back(nodeIndex - 1);
    }
    if (x < width) {
        neighbors.push_back(nodeIndex + 1);
    }
    if (y > 0) {
        neighbors.push_back(nodeIndex - width);
    }
    if (y < height) {
        neighbors.push_back(nodeIndex + width);
    }
    return neighbors;
}