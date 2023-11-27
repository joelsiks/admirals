#include "Scene.hpp"
#include <stdio.h>

using namespace admirals::scene;

void Scene::AddObject(std::shared_ptr<GameObject> object) {
    this->m_objects.Insert(std::move(object));
}

void Scene::Render(const EngineContext &ctx) const {
    for (const auto &object : this->m_objects) {
        object->Render(ctx);
    }
}

/**
 * receives a shared pointer to a gameObject and removes one from the multiset
 */
void Scene::RemoveObject(std::shared_ptr<GameObject> object) {
    if (ExistObject(object)) {
        this->m_objects.Erase(object->name());
    }
}
void Scene::RemoveObject(const std::string &key) { this->m_objects.Erase(key); }

/**
 * receives a shared pointer to a gameObject and checks if it exist in the
 * multiset
 */
bool Scene::ExistObject(std::shared_ptr<GameObject> object) {
    auto obj = this->m_objects.Find(object->name());
    if (obj != nullptr) {
        // if another that doesn't exist in m_objects have an identical name to
        // on e that does. check if the pointer is the same as the desired
        // value.
        if (obj == object) {
            return true;
        }
    }
    return false;
}
bool Scene::ExistObject(const std::string &key) {
    return (this->m_objects.Find(key) != nullptr);
}

int Scene::NumObjectsInScene() { return this->m_objects.Size(); }

std::vector<std::string> Scene::GetSceneObjectNames() {
    std::vector<std::string> vec = {};
    for (const auto &value : this->m_objects) {
        vec.emplace_back(value->name());
    }
    return vec;
}

void Scene::OnStart(const EngineContext &ctx) {
    m_isInitialized = true;

    for (const auto &object : this->m_objects) {
        object->OnStart(ctx);
    }
}

void Scene::OnUpdate(const EngineContext &ctx) {
    for (const auto &object : this->m_objects) {
        object->OnUpdate(ctx);
    }
}
