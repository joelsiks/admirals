#include "Scene.hpp"

using namespace admirals::scene;

void Scene::render(const renderer::RendererContext &r) const {
    for (const auto &object : this->m_collection) {
        object->onUpdate();
        object->render(r);
    }
}

void Scene::addObject(const std::shared_ptr<GameObject> object) {
    this->m_collection.insert(std::move(object));
}