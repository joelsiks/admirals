#pragma once

#include <memory>
#include <unordered_set>

#include "IDrawable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "UI/Element.hpp"
#include "events/EventSystem.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals::UI {

class DisplayLayout : public renderer::IDrawable {
public:
    // Engine events
    void Render(const EngineContext &ctx) const override;

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    // Collection
    void AddElement(std::shared_ptr<Element> element);

    static Vector2 GetPositionFromOrientation(DisplayOrientation orientation,
                                              const Vector2 &displaySize,
                                              const EngineContext &ctx);

    // Initialization
    void RebuildQuadTree(const Vector2 &windowSize);

protected:
    OrderedCollection<Element> m_elements;
    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
};

} // namespace admirals::UI