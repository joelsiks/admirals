#pragma once
#include <set>
#include <vector>
#include "GameObject.hpp"
#include "IDrawable.hpp"

namespace admirals {
namespace scene {

class Scene : public renderer::IDrawable
{
private:
    // should be sorted with respect to game object index, lower first.
    struct GameObjectComparator
    {
        bool operator()(const GameObject* l, const GameObject* r) const
        {
            return l->index() < r->index();
        }
    };

    std::multiset<GameObject*, GameObjectComparator> objects;
    
public:
    Scene();
    ~Scene();

    void render() const;
    void addObject(GameObject *object);
    std::vector<GameObject*> getObjects() const;
};

}}