
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stack>

#include "QuadTree.hpp"

using namespace admirals;

//  The QuadTree is indexed in the following manner:
//
//  0 | 1
//  -----
//  2 | 3
enum Quadrant : int { TopLeft = 0, TopRight, BottomLeft, BottomRight };

static Quadrant GetQuadrantFromPosition(const Vector2 &position,
                                        const Vector2 &origin,
                                        const Vector2 &size) {
    // Calculate the center point of the quadrant
    const Vector2 center = origin + (size / 2.0);

    // Determine the quadrant based on the position relative to the center
    if (position.x() < center.x()) {
        if (position.y() < center.y()) {
            return Quadrant::TopLeft;
        }
        return Quadrant::BottomLeft;
    }

    if (position.y() < center.y()) {
        return Quadrant::TopRight;
    }

    return Quadrant::BottomRight;
}

static inline Vector2 CalculateNewOrigin(Quadrant q, const Vector2 &origin,
                                         const Vector2 &size) {
    switch (q) {
    case Quadrant::TopLeft:
        return origin;
    case Quadrant::TopRight:
        return origin + Vector2(size.x(), 0);
    case Quadrant::BottomLeft:
        return origin + Vector2(0, size.y());
    case Quadrant::BottomRight:
        return origin + size;
    default:
        return origin;
    }
}

// Origin is the top left corner of the overlying box, and size is the size of
// one quadrant inside that box.
static bool inline InQuadrant(const std::shared_ptr<IDisplayable> &object,
                              Quadrant q, const Vector2 &origin,
                              const Vector2 &size) {

    const Vector2 newOrigin = CalculateNewOrigin(q, origin, size);
    return object->IsPartiallyInsideBox(newOrigin, size);
}

QuadTree::QuadTree() {}

QuadTree::~QuadTree() { DestroyTree(); }

std::vector<std::shared_ptr<IDisplayable>>
QuadTree::GetObjectsAtPosition(const Vector2 &position) const {
    Vector2 currentSize = m_size;
    Node *prev = nullptr;
    Node *current = m_rootNode;

    while (current != nullptr) {
        currentSize /= 2.0;
        prev = current;

        const Quadrant q =
            GetQuadrantFromPosition(position, current->origin, currentSize);
        current = current->quadrants[q];
    }

    if (prev == nullptr) {
        return std::vector<std::shared_ptr<IDisplayable>>();
    }

    return prev->data;
}

void QuadTree::BuildTree(
    const Vector2 &windowSize,
    const std::vector<std::shared_ptr<IDisplayable>> &objects) {

    // Destroy the previous tree, if constructed.
    DestroyTree();

    // Filter out objects that are not inside the window.
    std::vector<std::shared_ptr<IDisplayable>> filteredObjects;
    std::copy_if(
        objects.begin(), objects.end(), std::back_inserter(filteredObjects),
        [&windowSize](const std::shared_ptr<IDisplayable> &object) {
            return object->IsPartiallyInsideBox(Vector2(0), windowSize);
        });

    // Initialize values.
    m_rootNode = new Node();
    m_size = windowSize;

    // Construct initial build data object.
    const BuildData data = {filteredObjects, m_rootNode, Vector2(0), m_size};
    m_buildQueue.push(data);

    while (!m_buildQueue.empty()) {
        const BuildData data = m_buildQueue.front();
        m_buildQueue.pop();

        std::vector<std::shared_ptr<IDisplayable>> quadrantObjects[4];
        const Vector2 newSize = data.size / 2.f;
        int numEncapsulatingObjects = 0;

        for (const auto &object : data.objects) {
            // If the quadrant contains overlapping objects that all encapsulate
            // the quadrant, there is no point in dividing it further. Prevents
            // an infinite loop.
            if (object->IsEncapsulatingBox(data.origin, data.size)) {
                numEncapsulatingObjects++;
            }

            if (InQuadrant(object, Quadrant::TopLeft, data.origin, newSize)) {
                quadrantObjects[Quadrant::TopLeft].push_back(object);
            }

            if (InQuadrant(object, Quadrant::TopRight, data.origin, newSize)) {
                quadrantObjects[Quadrant::TopRight].push_back(object);
            }

            if (InQuadrant(object, Quadrant::BottomLeft, data.origin,
                           newSize)) {
                quadrantObjects[Quadrant::BottomLeft].push_back(object);
            }

            if (InQuadrant(object, Quadrant::BottomRight, data.origin,
                           newSize)) {
                quadrantObjects[Quadrant::BottomRight].push_back(object);
            }
        }

        // If the current box is encapsulated by all objects, we set the current
        // level to those objects and break.
        if (numEncapsulatingObjects == data.objects.size()) {
            data.node->data = data.objects;
            continue;
        }

        for (int i = 0; i < NUM_QUADRANTS; i++) {
            const size_t numObjectsInQuadrant = quadrantObjects[i].size();

            const Vector2 newOrigin = CalculateNewOrigin(
                static_cast<Quadrant>(i), data.origin, newSize);

            if (numObjectsInQuadrant > 0) {
                data.node->quadrants[i] = new Node();
                data.node->quadrants[i]->origin = newOrigin;
            }

            if (numObjectsInQuadrant == 1) {
                data.node->quadrants[i]->data = quadrantObjects[i];
            } else if (numObjectsInQuadrant > 1) {
                m_buildQueue.push({
                    quadrantObjects[i],
                    data.node->quadrants[i],
                    newOrigin,
                    newSize,
                });
            }
        }
    }
}

void QuadTree::DestroyTree() {
    if (m_rootNode == nullptr) {
        return;
    }

    std::stack<Node *> destroyStack;
    destroyStack.push(m_rootNode);

    while (!destroyStack.empty()) {
        const Node *currentNode = destroyStack.top();
        destroyStack.pop();

        // If the data is empty, the node is not a leaf.
        for (size_t i = 0; i < NUM_QUADRANTS; i++) {
            if (currentNode->quadrants[i] != nullptr) {
                destroyStack.push(currentNode->quadrants[i]);
            }
        }

        delete currentNode;
    }
}

void QuadTree::PrintNode(const Node *node, int depth, Vector2 size) const {
    if (node == nullptr) {
        return;
    }

    size /= 2.0f;

    // Print information about the current node
    for (int i = 0; i < depth; ++i) {
        std::cout
            << "  "; // Adjust indentation for tree structure visualization
    }
    std::cout << "Node at (" << node->origin.x() << ", " << node->origin.y()
              << "), size(" << size.x() << ", " << size.y() << ") ";

    if (!node->data.empty()) {
        std::cout << " [Leaf Node with " << node->data.size()
                  << " object(s)]\n";
    } else {
        std::cout << " [Internal Node]\n";
        // Recursively print its children
        for (int i = 0; i < NUM_QUADRANTS; ++i) {
            PrintNode(node->quadrants[i], depth + 1, size);
        }
    }
}