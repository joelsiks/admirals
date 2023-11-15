#pragma once

#include <memory>
#include <set>
#include <vector>

#include "GameObject.hpp"
#include "IDrawable.hpp"

namespace admirals {
namespace scene {

class Scene : public renderer::IDrawable {
private:
    // Should be sorted with respect to game object index, lower first.
    struct GameObjectComparator {
        bool operator()(const std::shared_ptr<GameObject> l,
                        const std::shared_ptr<GameObject> r) const {
            return l->index() < r->index();
        }
    };

    std::multiset<std::shared_ptr<GameObject>, GameObjectComparator> objects;

    int gridLenght;
    int gridHeight;
    bool dupsAllowed;

public:
    Scene();
    ~Scene();

    void render() const;
    void addObject(std::shared_ptr<GameObject> object);
    void removeOneObject(std::shared_ptr<GameObject> object);
    void removeAllObject(std::shared_ptr<GameObject> object);
    bool existObject(std::shared_ptr<GameObject> object);

    int sizeObjects();
    void setDupsAllowed(bool statement);

    // bool collisionObject(std::shared_ptr<GameObject> object);
    // std::vector<GameObject> detectObject(std::shared_ptr<GameObject>, int
    // range);
};

} // namespace scene
} // namespace admirals