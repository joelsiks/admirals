#include "Scene.hpp"

using namespace admirals::scene;

void Scene::Render(const renderer::RendererContext &r) const {
    for (const auto &object : this->m_objects) {
        object->OnUpdate();
        object->Render(r);
    }
}

void Scene::AddObject(std::shared_ptr<GameObject> object) {
    this->m_objects.Insert(std::move(object));
}