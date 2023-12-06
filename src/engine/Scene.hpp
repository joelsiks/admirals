#pragma once

#include <deque>
#include <functional>
#include <unordered_set>

#include "GameObject.hpp"
#include "IDisplayLayer.hpp"
#include "NavMesh.hpp"

namespace admirals {

class Scene : public IDisplayLayer {
public:
    typedef std::function<bool(
        const Rect &,
        const std::unordered_set<std::shared_ptr<IInteractiveDisplayable>> &)>
        PathValidator;

    virtual void Render(const EngineContext &ctx) const override;

    virtual void OnStart(const EngineContext &ctx);
    virtual void OnUpdate(const EngineContext &ctx);

    bool IsInitialized() const { return m_isInitialized; }

    /// @brief Builds a nav-mesh used in path-finding for objects on the scene.
    /// @param bounds The bounds to build the nav-mesh inside
    /// @param pathSize The width and height of the path, used when checking
    /// collisions.
    /// @param detailLevel The size of each individual step in the path.
    /// @param validator Function used to determine if a location on the mesh is
    /// @return A queue containing the steps to take from the root node to the
    /// node, or an empty queue if no valid path exists.
    std::shared_ptr<NavMesh> BuildNavMesh(const Rect &bounds,
                                          const Vector2 &pathSize,
                                          float detailLevel,
                                          const PathValidator &validator) const;

private:
    bool m_isInitialized;
};

} // namespace admirals