#include "Scene.hpp"

using namespace admirals::scene;

void Scene::render() const {
    for (auto &object : this->m_objects) {
        object->onUpdate();
        object->render();
    }
}

void Scene::addObject(std::shared_ptr<GameObject> object) {
    this->m_objects.insert(object);
}

Scene::Scene() { this->m_objects = {}; }

Scene::~Scene() {}