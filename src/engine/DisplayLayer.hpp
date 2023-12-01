#pragma once

#include <unordered_set>

#include "IDisplayable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "UI/Element.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class DisplayLayer : public IDisplayable {
public:
    // Engine events
    void Render(const EngineContext &ctx) const = 0;

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    // Collection
    // void AddElement(std::shared_ptr<IDisplayable> element);

    static Vector2
    GetPositionFromOrientation(UI::DisplayOrientation orientation,
                               const Vector2 &displaySize,
                               const EngineContext &ctx);

    // Initialization
    void RebuildQuadTree(const Vector2 &windowSize);

protected:
    OrderedCollection<IDisplayable> m_displayables;

    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
};

} // namespace admirals
