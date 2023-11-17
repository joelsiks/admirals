#include "Scene.hpp"
#include <stdio.h>

using namespace admirals::scene;

void Scene::AddObject(std::shared_ptr<GameObject> object) {
    this->m_objects.Insert(std::move(object));
}

void Scene::Render(const renderer::RendererContext &r) const {
    for (const auto &object : this->m_objects) {
        object->Render(r);
    }
}

/**
 * recieves a shared pointer to a gameObject and removes one from the multiset
 */
void Scene::RemoveObject(std::shared_ptr<GameObject> object) {
    this->m_objects.Erase(object->name());
    /*
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
    */
}

/**
 * recieves a shared pointer to a gameObject and checks if it exist in the
 * multiset
 */
bool Scene::ExistObject(std::shared_ptr<GameObject> object) {
    return (this->m_objects.Find(object->name()) != nullptr);
    /*
    
    for (auto it = this->m_objects.begin(); it != this->m_objects.end(); it++) {
        std::shared_ptr<GameObject> obj = *it;
        if (obj.get() == object.get()) {
            if (obj.get()->position().z() == object.get()->position().z()) {
                return true;
            }
        }
    }
    return false;
    */
}

void Scene::OnStart() {
    for (const auto &object : this->m_objects) {
        object->OnStart();
    }
}

void Scene::OnUpdate() {
    for (const auto &object : this->m_objects) {
        object->OnUpdate();
    }
}

Scene::Scene() { this->m_objects = {}; }

Scene::~Scene() {}
