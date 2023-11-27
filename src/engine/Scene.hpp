#pragma once

#include <memory>
#include <set>
#include <vector>

#include "GameObject.hpp"
#include "IDrawable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"

namespace admirals::scene {

class Scene : public renderer::IDrawable {
public:
    void AddObject(std::shared_ptr<GameObject> object);
    void RemoveObject(const std::shared_ptr<GameObject> &object);
    void RemoveObject(const std::string &key);
    bool ExistObject(const std::shared_ptr<GameObject> &object);
    bool ExistObject(const std::string &key);
    void Render(const EngineContext &ctx) const override;
    size_t NumObjectsInScene();

    std::vector<std::string> GetSceneObjectNames();
    void OnStart(const EngineContext &ctx);
    void OnUpdate(const EngineContext &ctx);

    bool IsInitialized() const { return m_isInitialized; }

    void RebuildQuadTree(const Vector2 &windowSize);

private:
    OrderedCollection<GameObject> m_objects;
    QuadTree m_quadtree;
    bool m_isInitialized;
};

} // namespace admirals::scene