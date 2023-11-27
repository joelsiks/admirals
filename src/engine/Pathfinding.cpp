#pragma once

#include "GameObject.cpp"
#include "Scene.hpp"
#include <List>
#include <vector>

namespace admirals {

struct PF_Node {
    Vector2 current_position;
    float distance;
    float heuristic;
    std::list<Vector2> node_list;
};

class Pathfinding {
private:
    std::vector<Vector2> m_adjacent_tiles_list =
        {
            {1, 0},  {-1, 0},
            {0, 1},  {0, -1}, // vertical and horisontal directions
            {1, 1},  {-1, -1},
            {1, -1}, {-1, 1} // diagonal directions
    };

    float heuristic(Vector2 start, Vector2 end) {
        // manhattan
        return std::abs(start.x() - end.x()) + std::abs(start.y() - end.y());
    }
    bool within(Vector2 gridSize, Vector2 position) {
        return within(gridSize, (int)position.x(), (int)position.y());
    }
    bool within(Vector2 gridSize, int position_x, int position_y) {
        return (gridSize.x() > position_x && 0 < position_x &&
                gridSize.y() > position_y && 0 < position_y);
    }

public:
    std::list<Vector2> findpath(Vector2 mapSize,
                                OrderedCollection<GameObject> gameObjects,
                                std::shared_ptr<GameObject> object,
                                Vector2 dest) {

        Vector2 object_pos = {2, 2};
        struct PF_Node expanded;
        expanded.current_position = object_pos;
        expanded.distance = 0;
        expanded.heuristic = heuristic(object_pos, dest);
        expanded.node_list = std::list<Vector2>();
        std::list<PF_Node> frontier;

        std::shared_ptr<GameObject> mapGrid[(int)mapSize.x()][(int)mapSize.y()];
        auto it = gameObjects.begin();
        for (it; it != gameObjects.end(); it++) {
            std::shared_ptr<GameObject> object = *it;
            mapGrid[(int)object->GetPosition().x()]
                   [(int)object->GetPosition().y()] = object;
        }

        while (expanded.heuristic > 0) {
            for (size_t i = 0; i < 8; i++) {
                Vector2 nextNode = {expanded.current_position.x(),
                                    expanded.current_position.y()};
                nextNode += m_adjacent_tiles_list[i];

                if (within(mapSize, nextNode)) {
                    // TODO need to check with objects
                    // if (within(mapSize, nextNode) &&
                    // mapGrid[(int)nextNode.x()][(int)nextNode.y()] != nullptr)
                    // {
                    float pathCost = 1;
                    if (i > 3) {
                        pathCost = 1.99f;
                    }
                    float node_heuristic = heuristic(nextNode, dest);
                    bool newNode = true;
                    bool replaceNode = false;
                    auto it = frontier.begin();

                    if (frontier.size() > 0) {
                        for (it; it != frontier.end(); it++) {
                            PF_Node temp_node = *it;
                            if (nextNode == temp_node.current_position) {
                                float oldCost =
                                    temp_node.distance + temp_node.heuristic;
                                float newCost = pathCost + node_heuristic;
                                if (newCost < oldCost) {
                                    replaceNode = true;
                                } else {
                                    newNode = false;
                                }
                                break;
                            }
                        }
                    }
                    if (newNode) {
                        if (replaceNode) {
                            frontier.erase(it);
                        }
                        PF_Node newFrontier;
                        newFrontier.current_position = nextNode;
                        newFrontier.distance = pathCost;
                        newFrontier.heuristic = node_heuristic;
                        newFrontier.node_list =
                            std::list<Vector2>(expanded.node_list);
                        newFrontier.node_list.emplace_back(
                            expanded.current_position);
                        frontier.emplace_back(newFrontier);
                    }
                }
            }
            auto nextExp = frontier.begin();
            float totalCost = 10000;
            auto it = frontier.begin();
            for (it; it != frontier.end(); it++) {
                PF_Node temp_node = *it;
                float cost = temp_node.distance + temp_node.heuristic;
                if (cost < totalCost) {
                    totalCost = cost;
                    nextExp = it;
                }
            }
            expanded = *nextExp;
            frontier.erase(nextExp);
        }

        expanded.node_list.pop_front();
        expanded.node_list.emplace_back(expanded.current_position);
        auto it_exp = expanded.node_list.begin();
        for (it_exp; it_exp != expanded.node_list.end(); it_exp++) {
            std::cout << "x:" << (*it_exp).x() << ", y:" << (*it_exp).y()
                      << "\n";
        }
        return expanded.node_list;
    }

    void visualizeGrid(Vector2 mapSize,
                       OrderedCollection<GameObject> gameObjects) {

        for (size_t i = 0; i < mapSize.x(); i++) {
            for (size_t j = 0; j < mapSize.y(); j++) {

                std::cout << "| ";
                auto it = gameObjects.begin();
                bool found = false;
                for (it; it != gameObjects.end(); it++) {
                    std::shared_ptr<GameObject> object = *it;
                    if (object->GetPosition().x() == i &&
                        object->GetPosition().y() == j) {
                        found = true;
                        std::cout << "@";
                    }
                }
                if (!found) {
                    std::cout << " ";
                }
                std::cout << " ";
            }
            std::cout << "\n";
        }
    }
};

} // namespace admirals