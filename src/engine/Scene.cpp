#include "Scene.hpp"
#include "IInteractiveDisplayable.hpp"
#include "PathFinding.hpp"

using namespace admirals;

void Scene::Render(const EngineContext &ctx) const {
    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        object->Render(ctx);
    }

    if (ctx.debug) {
        m_quadtree.DrawTree();
    }
}

void Scene::OnStart(const EngineContext &ctx) {
    m_isInitialized = true;

    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        object->OnStart(ctx);
    }
}

void Scene::OnUpdate(const EngineContext &ctx) {
    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        object->OnUpdate(ctx);
    }
}

std::deque<Vector2> Scene::FindPath(
    const Vector2 &start, const Vector2 &dest, const Vector2 &pathSize,
    const std::unordered_set<float> &checkedOrders, float detailLevel) const {
    return PathFinding::FindPath(m_quadtree, start, dest, pathSize,
                                 checkedOrders, detailLevel);
}
