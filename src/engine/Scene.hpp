#pragma once

#include <deque>
#include <memory>
#include <unordered_set>
#include <vector>

#include "GameObject.hpp"
#include "IInteractiveDrawable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "events/EventSystem.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals::scene {

class Scene : public renderer::IDrawable {
public:
    // Engine events
    virtual void Render(const EngineContext &ctx) const override;

    virtual void OnStart(const EngineContext &ctx);
    virtual void OnUpdate(const EngineContext &ctx);

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    // Initialization
    bool IsInitialized() const { return m_isInitialized; }
    void RebuildQuadTree(const Vector2 &windowSize);

    // Collection
    void AddObject(std::shared_ptr<GameObject> object);
    void RemoveObject(const std::shared_ptr<GameObject> &object);
    void RemoveObject(const std::string &key);
    bool ExistObject(const std::shared_ptr<GameObject> &object);
    bool ExistObject(const std::string &key);
    size_t NumObjectsInScene();
    std::vector<std::string> GetSceneObjectNames();

    std::deque<Vector2> FindPath(const Vector2 &start, const Vector2 &dest,
                                 const Vector2 &pathSize,
                                 const std::unordered_set<float> &checkedOrders,
                                 float detailLevel) const;

private:
    OrderedCollection<GameObject> m_objects;
    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
    bool m_isInitialized;
};

} // namespace admirals::scene