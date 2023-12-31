#pragma once

#include <unordered_set>
#include <variant>
#include <vector>

#include "DeferedAction.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals {

class IDisplayLayer {
public:
    virtual void Render(const EngineContext &ctx) const = 0;
    virtual void Update(const EngineContext &ctx);

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    virtual void
    AddDisplayable(std::shared_ptr<IInteractiveDisplayable> displayable);
    virtual void RemoveDisplayable(const std::string &identifier);
    virtual bool ExistsDisplayable(const std::string &identifier);
    virtual std::shared_ptr<IInteractiveDisplayable>
    FindDisplayable(const std::string &identifier);

    inline size_t NumDisplayables() const { return m_displayables.Size(); }
    std::vector<std::string> GetDisplayableIdentifiers();

    void RebuildQuadTree(const Vector2 &windowSize);
    inline const QuadTree &GetQuadTree() const { return m_quadtree; }

protected:
    typedef std::variant<std::string, std::shared_ptr<IInteractiveDisplayable>>
        DeferredValueType;

    void HandleDeferredActions();

    OrderedCollection<IInteractiveDisplayable> m_displayables;

    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
    std::vector<std::pair<DeferType, DeferredValueType>> m_deferredActions;
};

} // namespace admirals
