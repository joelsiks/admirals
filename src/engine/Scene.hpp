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
            return l->position().z() < r->position().z();
        }
    };

    std::multiset<std::shared_ptr<GameObject>, GameObjectComparator> m_objects;

public:
    Scene();
    ~Scene();

    void render(const renderer::RendererContext &r) const;
    void addObject(std::shared_ptr<GameObject> object);
};

} // namespace scene
} // namespace admirals