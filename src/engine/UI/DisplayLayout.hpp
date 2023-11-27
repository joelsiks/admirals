#pragma once

#include <memory>
#include <vector>

#include "IInteractiveDrawable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public IInteractiveDrawable {
public:
    void Render(const EngineContext &ctx) const override;

    virtual void OnClick(events::MouseClickEventArgs &args) override;
    virtual void OnMouseEnter(events::EventArgs &args) override;
    virtual void OnMouseLeave(events::EventArgs &args) override;
    virtual void OnMouseMove(events::EventArgs &args) override;

    void AddElement(std::shared_ptr<Element> element);

    void RebuildQuadTree(const Vector2 &windowSize);

    static Vector2 GetPositionFromOrientation(DisplayOrientation orientation,
                                              const Vector2 &displaySize,
                                              const EngineContext &ctx);

protected:
    OrderedCollection<Element> m_elements;
    QuadTree m_quadtree;
};

} // namespace admirals::UI