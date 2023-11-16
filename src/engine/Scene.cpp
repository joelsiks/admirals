#include "Scene.hpp"
#include <stdio.h>

namespace admirals {
namespace scene {

void Scene::render() const {
    for (auto &object : this->objects) {
        object->onUpdate();
        object->render();
    }
}

/**
 * recieves a shared pointer to a gamObject and insert it to the multiset.
 */
void Scene::addObject(std::shared_ptr<GameObject> object) {
    this->objects.insert(object);
}

/**
 * recieves a shared pointer to a gameObject and removes one from the multiset
 */
void Scene::removeObject(std::shared_ptr<GameObject> object) {
    auto it = this->objects.begin();
    bool found = false;
    for (it; it != this->objects.end(); it++) {
        std::shared_ptr<GameObject> obj = *it;
        if (obj.get() == object.get()) {
            if (obj.get()->index() == object.get()->index()) {
                found = true;
                break;
            }
        }
    }
    if (found) {
        this->objects.erase(it);
    }
}

/**
 * recieves a shared pointer to a gameObject and checks if it exist in the multiset
 */
bool Scene::existObject(std::shared_ptr<GameObject> object) {
    for (auto it = this->objects.begin(); it != this->objects.end(); it++) {
        std::shared_ptr<GameObject> obj = *it;
        if (obj.get() == object.get()) {
            if (obj.get()->index() == object.get()->index()) {
                return true;
            }
        }
    }
    return false;
    
}

/**
 * returns the total amount of shared pointers to objects currently in the multiset
 */
int Scene::numObjectsInScene() { return this->objects.size(); }

Scene::Scene() {
    this->objects = {};
}

Scene::~Scene() {}

} // namespace scene
} // namespace admirals