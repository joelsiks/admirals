#include <stdio.h>

#include "IInteractiveDisplayable.hpp"
#include "PathFinding.hpp"
#include "Scene.hpp"

using namespace admirals;
using namespace admirals::scene;

void Scene::AddObject(std::shared_ptr<GameObject> object) {
    this->m_objects.Insert(std::move(object));
}

void Scene::Render(const EngineContext &ctx) const {
    for (const auto &object : this->m_objects) {
        object->Render(ctx);
    }

    if (ctx.debug) {
        m_quadtree.DrawTree();
    }
}

/**
 * receives a shared pointer to a gameObject and removes one from the multiset
 */
void Scene::RemoveObject(const std::shared_ptr<GameObject> &object) {
    if (ExistObject(object)) {
        this->m_objects.Erase(object->name());
    }
}
void Scene::RemoveObject(const std::string &key) { this->m_objects.Erase(key); }

/**
 * receives a shared pointer to a gameObject and checks if it exist in the
 * multiset
 */
bool Scene::ExistObject(const std::shared_ptr<GameObject> &object) {
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

size_t Scene::NumObjectsInScene() { return this->m_objects.Size(); }

std::vector<std::string> Scene::GetSceneObjectNames() {
    std::vector<std::string> vec = {};
    for (const auto &value : this->m_objects) {
        vec.emplace_back(value->name());
    }
    return vec;
}

std::deque<Vector2> Scene::FindPath(
    const Vector2 &start, const Vector2 &dest, const Vector2 &pathSize,
    const std::unordered_set<float> &checkedOrders, float detailLevel) const {
    return PathFinding::FindPath(m_quadtree, start, dest, pathSize,
                                 checkedOrders, detailLevel);
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

void Scene::OnClick(events::MouseClickEventArgs &args) {
    const Vector2 clickLocation = args.Location();
    for (const auto &object : m_quadtree.GetObjectsAtPosition(clickLocation)) {
        if (args.handled)
            break;
        auto obj = static_pointer_cast<GameObject>(object);
        if (obj->IsVisible() && obj->GetBoundingBox().Contains(clickLocation)) {
            obj->OnClick(args);
        }
    }
}

void Scene::OnMouseMove(events::MouseMotionEventArgs &args) {
    const Vector2 mouseLocation = args.Location();
    std::unordered_set<std::string> currentMouseOverObjects;

    for (const auto &object : m_quadtree.GetObjectsAtPosition(mouseLocation)) {
        if (args.handled) {
            break;
        }

        auto obj = static_pointer_cast<GameObject>(object);
        const std::string name = obj->name();
        if (obj->IsVisible() && obj->GetBoundingBox().Contains(mouseLocation)) {
            currentMouseOverObjects.insert(name);
            if (m_mouseOverSet.insert(name).second) {
                obj->OnMouseEnter(args);
            } else {
                obj->OnMouseMove(args);
            }
        }
    }

    // Handle objects that are no longer moused over
    for (auto it = m_mouseOverSet.begin(); it != m_mouseOverSet.end();) {
        if (args.handled) {
            break;
        }

        const std::string &name = *it;
        if (!currentMouseOverObjects.contains(name)) {
            auto obj = m_objects.Find(name);
            if (obj != nullptr) {
                obj->OnMouseLeave(args);
            }
            it = m_mouseOverSet.erase(it);
        } else {
            it++;
        }
    }
}

void Scene::OnShown() {
    for (const auto &obj : m_objects) {
        obj->OnShown();
    }
}

void Scene::OnHidden() {
    for (const auto &obj : m_objects) {
        obj->OnHidden();
    }
}

void Scene::RebuildQuadTree(const Vector2 &windowSize) {
    // build a QuadTree to find out what objects to handle click events on.
    std::vector<std::shared_ptr<IInteractiveDisplayable>> objects;
    for (const auto &element : m_objects) {
        objects.push_back(
            dynamic_pointer_cast<IInteractiveDisplayable>(element));
    }
    m_quadtree.BuildTree(windowSize, objects);
}
