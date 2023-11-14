#include "Scene.hpp"

namespace admirals {
namespace scene {

void Scene::render() const {
    for (GameObject *object : this->objects) {
        object->onUpdate();
        object->render();
    }
}

void Scene::addObject(GameObject *object) { this->objects.insert(object); }

std::vector<GameObject *> Scene::getObjects() const {
    std::vector<GameObject *> v{objects.begin(), objects.end()};
    return v;
}

Scene::Scene() { this->objects = {}; }

Scene::~Scene() {
    for (GameObject *object : this->objects) {
        free(object);
    }
}

} // namespace scene
} // namespace admirals