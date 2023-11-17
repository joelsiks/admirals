#include "Scene.hpp"
#include <stdio.h>

using namespace admirals::scene;

void Scene::render(const renderer::RendererContext &r) const {
    for (auto &object : this->m_objects) {
        object->onUpdate();
        object->render(r);
    }
}

/**
 * recieves a shared pointer to a gamObject and insert it to the multiset.
 */
void Scene::addObject(std::shared_ptr<GameObject> object) {
    this->m_objects.insert(object);
}

/**
 * recieves a shared pointer to a gameObject and removes one from the multiset
 */
void Scene::removeObject(std::shared_ptr<GameObject> object) {
    auto it = this->m_objects.begin();
    bool found = false;
    for (it; it != this->m_objects.end(); it++) {
        std::shared_ptr<GameObject> obj = *it;
        if (obj.get() == object.get()) {
            if (obj.get()->position().z() == object.get()->position().z()) {
                found = true;
                break;
            }
        }
    }
    if (found) {
        this->m_objects.erase(it);
    }
}

/**
 * recieves a shared pointer to a gameObject and checks if it exist in the
 * multiset
 */
bool Scene::existObject(std::shared_ptr<GameObject> object) {
    for (auto it = this->m_objects.begin(); it != this->m_objects.end(); it++) {
        std::shared_ptr<GameObject> obj = *it;
        if (obj.get() == object.get()) {
            if (obj.get()->position().z() == object.get()->position().z()) {
                return true;
            }
        }
    }
    return false;
}

/**
 * returns the total amount of shared pointers to objects currently in the
 * multiset
 */
int Scene::numObjectsInScene() { return this->m_objects.size(); }

Scene::Scene() { this->m_objects = {}; }

Scene::~Scene() {}
