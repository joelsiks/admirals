
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stack>

#include "QuadTree.hpp"
#include "Renderer.hpp"

using namespace admirals;

//  The QuadTree is indexed in the following manner:
//
//  0 | 1
//  -----
//  2 | 3
enum Quadrant : int { TopLeft = 0, TopRight, BottomLeft, BottomRight };

static Quadrant GetQuadrantFromPosition(const Vector2 &position,
                                        const Rect &boundingBox) {

    const Vector2 center = boundingBox.Center();

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

const Vector2 QuadTree::QuadrantOffsets[] = {
    Vector2(0, 0), // TopLeft
    Vector2(1, 0), // TopRight
    Vector2(0, 1), // BottomLeft
    Vector2(1, 1)  // BottomRight
};

QuadTree::QuadTree() {}

QuadTree::~QuadTree() { DestroyTree(); }

std::vector<std::shared_ptr<IDisplayable>>
QuadTree::GetObjectsAtPosition(const Vector2 &position) const {
    Rect currentBoundingBox(Vector2(0), m_size);

    Node *prev = nullptr;
    Node *current = m_rootNode;

    while (current != nullptr) {
        prev = current;

        const Quadrant q =
            GetQuadrantFromPosition(position, currentBoundingBox);

        current = current->quadrants[q];
        if (current != nullptr) {
            currentBoundingBox.SetPosition(current->origin);
            currentBoundingBox.SetSize(currentBoundingBox.Size() / 2.0f);
        }
    }

    if (prev == nullptr) {
        return std::vector<std::shared_ptr<IDisplayable>>();
    }

    return prev->data;
}

std::unordered_set<std::shared_ptr<IDisplayable>>
QuadTree::GetObjectsInArea(const Rect &location) const {
    Vector2 size = m_size;

    Node *current = nullptr;
    std::unordered_set<std::shared_ptr<IDisplayable>> found;
    std::deque<Node *> visitQueue = {m_rootNode};
    while (!visitQueue.empty()) {
        current = visitQueue.back();
        visitQueue.pop_back();

        size /= 2.0f;

        for (const auto &quadrant : current->quadrants) {
            if (quadrant == nullptr) {
                continue;
            }
            if (Rect(quadrant->origin, size).Overlaps(location)) {
                if (quadrant->data.empty()) {
                    visitQueue.push_back(quadrant);
                } else {
                    for (const auto &o : quadrant->data) {
                        found.insert(o);
                    }
                }
            }
        }
    }

    return found;
}

void QuadTree::BuildTree(
    const Vector2 &windowSize,
    const std::vector<std::shared_ptr<IDisplayable>> &objects) {

    // Destroy the previous tree, if constructed.
    DestroyTree();
    InitializeTree(windowSize, objects);

    while (!m_buildQueue.empty()) {
        const BuildData data = m_buildQueue.front();
        m_buildQueue.pop();

        std::vector<std::shared_ptr<IDisplayable>>
            quadrantObjects[NUM_QUADRANTS];

        const Vector2 upperQuadrantPosition = data.bounds.Position();
        const Vector2 dividedQuadrantSize = data.bounds.Size() / 2.f;

        // If the quadrant size is less than the minimum quadrant size we should
        // not divide further and stop here.
        if (dividedQuadrantSize.x() < MINIMUM_QUADRANT_SIZE ||
            dividedQuadrantSize.y() < MINIMUM_QUADRANT_SIZE) {
            data.node->data = data.objects;
            continue;
        }

        Rect quadrantBounds[NUM_QUADRANTS];
        for (int i = 0; i < NUM_QUADRANTS; i++) {
            quadrantBounds[i] =
                Rect(upperQuadrantPosition +
                         QuadrantOffsets[i] * dividedQuadrantSize,
                     dividedQuadrantSize);
        }

        int numEncapsulatingObjects = 0;

        for (const auto &object : data.objects) {
            const Rect objectBounds = object->GetBoundingBox();

            // If the quadrant contains overlapping objects that all encapsulate
            // the quadrant, there is no point in dividing it further. Prevents
            // an infinite loop.
            if (objectBounds.Contains(data.bounds)) {
                numEncapsulatingObjects++;
            }

            for (int i = 0; i < NUM_QUADRANTS; i++) {
                if (quadrantBounds[i].Overlaps(objectBounds)) {
                    quadrantObjects[i].push_back(object);
                }
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

            if (numObjectsInQuadrant > 0) {
                data.node->quadrants[i] = new Node();
                data.node->quadrants[i]->origin = quadrantBounds[i].Position();
            }

            if (numObjectsInQuadrant == 1) {
                data.node->quadrants[i]->data = quadrantObjects[i];
            } else if (numObjectsInQuadrant > 1) {
                m_buildQueue.push({
                    quadrantObjects[i],
                    data.node->quadrants[i],
                    quadrantBounds[i],
                });
            }
        }
    }
}

void QuadTree::InitializeTree(
    const Vector2 &windowSize,
    const std::vector<std::shared_ptr<IDisplayable>> &objects) {
    // Initialize values.
    m_rootNode = new Node();
    m_size = windowSize;
    const Rect windowBounds = Rect(Vector2(0), m_size);

    // Filter out objects that are not inside the window.
    std::vector<std::shared_ptr<IDisplayable>> filteredObjects;
    std::copy_if(objects.begin(), objects.end(),
                 std::back_inserter(filteredObjects),
                 [&windowBounds](const std::shared_ptr<IDisplayable> &object) {
                     return windowBounds.Overlaps(object->GetBoundingBox());
                 });

    // Construct initial build data object.
    const BuildData data = {filteredObjects, m_rootNode, windowBounds};
    m_buildQueue.push(data);
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

void QuadTree::PrintNode(const Node *node, int depth,
                         const Vector2 &size) const {
    if (node == nullptr) {
        return;
    }

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
            PrintNode(node->quadrants[i], depth + 1, size / 2.0f);
        }
    }
}

void QuadTree::DrawNode(const Node *node, int depth,
                        const Vector2 &size) const {
    if (node == nullptr) {
        return;
    }

    renderer::Renderer::DrawRectangleOutline(node->origin, size, 1.0,
                                             Color::RED);

    if (node->data.empty()) {
        for (int i = 0; i < NUM_QUADRANTS; ++i) {
            DrawNode(node->quadrants[i], depth + 1, size / 2.0f);
        }
    }
}