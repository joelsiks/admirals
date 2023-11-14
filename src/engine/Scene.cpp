#include "Scene.hpp"

namespace admirals {
namespace scene {

void Scene::render() const {
    for (auto &object : this->objects) {
        object->onUpdate();
        object->render();
    }
}

void Scene::addObject(std::shared_ptr<GameObject> object) {
    this->objects.insert(object);
}

Scene::Scene() { this->objects = {}; }

Scene::~Scene() {}

} // namespace scene
} // namespace admirals