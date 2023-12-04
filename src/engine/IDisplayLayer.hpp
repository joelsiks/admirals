#pragma once

#include <unordered_set>

#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals {

class IDisplayLayer {
public:
    virtual void Render(const EngineContext &ctx) const = 0;

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    virtual void
    AddDisplayable(std::shared_ptr<IInteractiveDisplayable> displayable);
    virtual void RemoveDisplayable(const std::string &identifier);
    virtual bool ExistsDisplayable(const std::string &identifier);

    inline size_t NumDisplayables() const { return m_displayables.Size(); }
    std::vector<std::string> GetDisplayableNames();

    void RebuildQuadTree(const Vector2 &windowSize);

protected:
    OrderedCollection<IInteractiveDisplayable> m_displayables;

    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
};

} // namespace admirals
