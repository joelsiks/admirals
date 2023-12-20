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
    virtual void Update(const EngineContext &ctx) override;

    virtual void OnStart(const EngineContext &ctx);
    virtual void OnUpdate(const EngineContext &ctx);

    bool IsInitialized() const { return m_isInitialized; }

    /// @brief Creates a new `GameObject` in the scene and returns a shared
    /// pointer to it.
    /// @tparam T The type of the `GameObject`, must extend the `GameObject`
    /// class
    /// @tparam ..._Args The types of the constructor arguments to the
    /// `GameObject` type
    /// @param ..._args The constructor arguments to the `GameObject` type
    /// @return A shared pointer to the newly created `GameObject`
    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeGameObject(_Args &&..._args) {
        auto object = std::make_shared<T>(std::forward<_Args>(_args)...);
        AddDisplayable(object);
        return object;
    }

    /// @brief Builds a nav-mesh used in path-finding for objects in the scene.
    /// @param bounds The bounds to build the nav-mesh inside
    /// @param detailLevel The size of each individual step in the path.
    /// @param validator Function used to determine if a location on the mesh is
    /// navigable or not.
    /// @return A pointer to the built nav-mesh
    std::shared_ptr<NavMesh> BuildNavMesh(const Rect &bounds, float detailLevel,
                                          const PathValidator &validator) const;

private:
    bool m_isInitialized;
};

} // namespace admirals