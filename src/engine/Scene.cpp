#include "Scene.hpp"
#include "Engine.hpp"
#include "PathFinding.hpp"

using namespace admirals;

void Scene::Render(const EngineContext &ctx) const {
    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        object->Render(ctx);
    }

    if (Engine::HasDebugMask(ctx.debug, EngineDebugMode::DebugEnabled |
                                            EngineDebugMode::DebugQuadTree)) {
        m_quadtree.DrawTree();
    }
}

void Scene::Update(const EngineContext &ctx) {
    IDisplayLayer::Update(ctx);
    OnUpdate(ctx);
    RebuildQuadTree(ctx.windowSize);
}

void Scene::OnStart(const EngineContext &ctx) {
    m_isInitialized = true;
    HandleDeferredActions();
    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        if (object != nullptr) {
            object->OnStart(ctx);
        }
    }
}

void Scene::OnUpdate(const EngineContext &ctx) {
    HandleDeferredActions();
    for (const auto &displayable : m_displayables) {
        auto object = std::dynamic_pointer_cast<GameObject>(displayable);
        if (object != nullptr) {
            object->OnUpdate(ctx);
        }
    }
}

std::shared_ptr<NavMesh>
Scene::BuildNavMesh(const Rect &bounds, float detailLevel,
                    const PathValidator &validator) const {
    const size_t width = static_cast<size_t>((bounds.Width() / detailLevel));
    const size_t height = static_cast<size_t>((bounds.Height() / detailLevel));
    const size_t gridSize = width * height;
    float *grid = new float[gridSize];

    for (size_t i = 0; i < gridSize; i++) {
        const Vector2 position =
            PathFinding::ConvertNodeIndexToVector(i, width, detailLevel) +
            bounds.Position();
        const Rect objectBounds = Rect(position, detailLevel - __FLT_EPSILON__);
        if (validator(objectBounds,
                      m_quadtree.GetObjectsInArea(objectBounds))) {
            grid[i] = detailLevel;
        } else {
            grid[i] = -1;
        }
    }

    return std::make_shared<NavMesh>(width, height, grid, detailLevel, bounds);
}