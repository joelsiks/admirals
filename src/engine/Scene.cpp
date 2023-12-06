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

std::shared_ptr<NavMesh>
Scene::BuildNavMesh(const Rect &bounds, const Vector2 &pathSize,
                    float detailLevel, const PathValidator &validator) const {
    const size_t width = static_cast<size_t>((bounds.Width() / detailLevel));
    const size_t height = static_cast<size_t>((bounds.Height() / detailLevel));
    const size_t gridSize = width * height;
    float *grid = new float[gridSize];

    for (size_t i = 0; i < gridSize; i++) {
        const Vector2 position =
            PathFinding::ConvertNodeIndexToVector(i, width, detailLevel) +
            bounds.Position();
        const Rect objectBounds = Rect(position, pathSize - __FLT_EPSILON__);
        if (validator(objectBounds,
                      m_quadtree.GetObjectsInArea(objectBounds))) {
            grid[i] = detailLevel;
        } else {
            grid[i] = -1;
        }
    }

    return std::make_shared<NavMesh>(width, height, grid, detailLevel, bounds);
}