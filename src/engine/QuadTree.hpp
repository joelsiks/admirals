#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "DataObjects.hpp"

namespace admirals {

// Function to check if a point is inside a box
static inline bool PointInsideBox(float x, float y, float left, float top,
                                  float right, float bottom) {
    return (x >= left && x <= right && y >= top && y <= bottom);
}

class IDisplayable {
public:
    inline Vector2 GetDisplayOrigin() const { return m_displayOrigin; };
    inline Vector2 GetDisplaySize() const { return m_displaySize; };

    inline bool IsPartiallyInsideBox(const Vector2 &origin,
                                     const Vector2 &size) const {
        // Our box
        const float left1 = m_displayOrigin.x();
        const float right1 = m_displayOrigin.x() + m_displaySize.x();
        const float top1 = m_displayOrigin.y();
        const float bottom1 = m_displayOrigin.y() + m_displaySize.y();

        // Other box
        const float left2 = origin.x();
        const float right2 = origin.x() + size.x();
        const float top2 = origin.y();
        const float bottom2 = origin.y() + size.y();

        // Check if any of the four corners of m_displaySize and m_displayOrigin
        // box are inside the box defined by origin and size
        const bool cornerInside =
            (PointInsideBox(left1, top1, left2, top2, right2, bottom2) ||
             PointInsideBox(left1, bottom1, left2, top2, right2, bottom2) ||
             PointInsideBox(right1, top1, left2, top2, right2, bottom2) ||
             PointInsideBox(right1, bottom1, left2, top2, right2, bottom2));

        return cornerInside || IsInsideBox(origin, size);
    }

    inline bool IsInsideBox(const Vector2 &origin, const Vector2 &size) const {
        // Our box
        const float left1 = m_displayOrigin.x();
        const float right1 = m_displayOrigin.x() + m_displaySize.x();
        const float top1 = m_displayOrigin.y();
        const float bottom1 = m_displayOrigin.y() + m_displaySize.y();

        // Other box
        const float left2 = origin.x();
        const float right2 = origin.x() + size.x();
        const float top2 = origin.y();
        const float bottom2 = origin.y() + size.y();

        // Check if m_displaySize and m_displayOrigin box is encapsulated by the
        // other box
        return (left1 >= left2 && right1 <= right2 && top1 >= top2 &&
                bottom1 <= bottom2);
    }

    inline bool IsEncapsulatingBox(const Vector2 &origin,
                                   const Vector2 &size) const {
        // Our box
        const float left2 = m_displayOrigin.x();
        const float right2 = m_displayOrigin.x() + m_displaySize.x();
        const float top2 = m_displayOrigin.y();
        const float bottom2 = m_displayOrigin.y() + m_displaySize.y();

        // Other box
        const float left1 = origin.x();
        const float right1 = origin.x() + size.x();
        const float top1 = origin.y();
        const float bottom1 = origin.y() + size.y();

        // Check if m_displaySize and m_displayOrigin box is encapsulated by the
        // other box
        return (left1 >= left2 && right1 <= right2 && top1 >= top2 &&
                bottom1 <= bottom2);
    }

protected:
    Vector2 m_displayOrigin;
    Vector2 m_displaySize;
};

class QuadTree {
public:
    QuadTree();
    ~QuadTree();

    /// @brief This builds the QuadTree given a window size and a list of
    /// objects to place inside the tree.
    /// @param windowSize The size of the (SDL) window where objects can be
    /// placed.
    /// @param objects A vector
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
        Vector2 origin, size;
    };
    std::queue<BuildData> m_buildQueue;

    void DestroyTree();

    // Helper function to print a node and its children recursively
    void PrintNode(const Node *node, int depth, Vector2 size) const;

    Vector2 m_size = Vector2(0);
    Node *m_rootNode = nullptr;
};
} // namespace admirals