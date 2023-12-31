#pragma once

#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "IInteractiveDisplayable.hpp"

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
    void BuildTree(
        const Vector2 &windowSize,
        const std::vector<std::shared_ptr<IInteractiveDisplayable>> &objects);

    /// @brief Gets objects at a position inside the QuadTree
    /// @param position The position to check against in the QuadTree
    /// @return A vector of objects that overlap the position
    std::vector<std::shared_ptr<IInteractiveDisplayable>>
    GetObjectsAtPosition(const Vector2 &position) const;

    /// @brief Gets objects at a location inside the QuadTree
    /// @param location The location to check against in the QuadTree
    /// @return A vector of objects that overlap the position
    std::unordered_set<std::shared_ptr<IInteractiveDisplayable>>
    GetObjectsInArea(const Rect &location) const;

    inline void PrintTree() const {
        if (m_rootNode != nullptr && m_size != Vector2(0)) {
            PrintNode(m_rootNode, 0, m_size);
        }
    }

    inline void DrawTree() const {
        if (m_rootNode != nullptr && m_size != Vector2(0)) {
            DrawNode(m_rootNode, 0, m_size);
        }
    }

    inline Vector2 GetSize() const { return m_size; }

private:
    static constexpr int NUM_QUADRANTS = 4;
    static constexpr float MINIMUM_QUADRANT_SIZE = 10.f;
    static const Vector2 QuadrantOffsets[NUM_QUADRANTS];

    // If data is not a nullptr, the Node is a leaf.
    // If data is a nullptr, the quadrants are populated instead.
    struct Node {
        Vector2 origin;
        std::vector<std::shared_ptr<IInteractiveDisplayable>> data;
        Node *quadrants[NUM_QUADRANTS] = {nullptr};
    };

    struct BuildData {
        std::vector<std::shared_ptr<IInteractiveDisplayable>> objects;
        Node *node;
        Rect bounds;
    };
    std::queue<BuildData> m_buildQueue;

    void InitializeTree(
        const Vector2 &windowSize,
        const std::vector<std::shared_ptr<IInteractiveDisplayable>> &objects);

    void DestroyTree();

    // Helper function to print a node and its children recursively
    void PrintNode(const Node *node, int depth, const Vector2 &size) const;
    void DrawNode(const Node *node, int depth, const Vector2 &size) const;

    Vector2 m_size = Vector2(0);
    Node *m_rootNode = nullptr;
};
} // namespace admirals