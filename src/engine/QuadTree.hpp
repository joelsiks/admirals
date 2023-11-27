#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "IDisplayable.hpp"

namespace admirals {

class QuadTree {
public:
    QuadTree();
    ~QuadTree();

    /// @brief This builds the QuadTree given a window size and a vector of
    /// objects to place inside the tree.
    /// @param windowSize The size of the (SDL) window where objects can be
    /// placed.
    /// @param objects A vector containing all objects to place inside the tree.
    void BuildTree(const Vector2 &windowSize,
                   const std::vector<std::shared_ptr<IDisplayable>> &objects);

    /// @brief Gets objects at a position inside the QuadTree
    /// @param position The position to check against in the QuadTree
    /// @return A vector of objects that overlap the position
    std::vector<std::shared_ptr<IDisplayable>>
    GetObjectsAtPosition(const Vector2 &position) const;

    inline void PrintTree() const {
        if (m_rootNode != nullptr && m_size != Vector2(0)) {
            PrintNode(m_rootNode, 0, m_size);
        }
    }

private:
    static constexpr int NUM_QUADRANTS = 4;
    static constexpr float MINIMUM_QUADRANT_SIZE = 1.f;
    static const Vector2 QuadrantOffsets[NUM_QUADRANTS];

    // If data is not a nullptr, the Node is a leaf.
    // If data is a nullptr, the quadrants are populated instead.
    struct Node {
        Vector2 origin;
        std::vector<std::shared_ptr<IDisplayable>> data;
        Node *quadrants[NUM_QUADRANTS] = {nullptr};
    };

    struct BuildData {
        std::vector<std::shared_ptr<IDisplayable>> objects;
        Node *node;
        Rect bounds;
    };
    std::queue<BuildData> m_buildQueue;

    void
    InitializeTree(const Vector2 &windowSize,
                   const std::vector<std::shared_ptr<IDisplayable>> &objects);

    void DestroyTree();

    // Helper function to print a node and its children recursively
    void PrintNode(const Node *node, int depth, Vector2 size) const;

    Vector2 m_size = Vector2(0);
    Node *m_rootNode = nullptr;
};
} // namespace admirals