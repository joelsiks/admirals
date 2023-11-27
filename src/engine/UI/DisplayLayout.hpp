#pragma once

#include <memory>
#include <vector>

#include "InteractiveDrawable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public InteractiveDrawable {
public:
    void Render(const EngineContext &ctx) const override;

    virtual void OnClick(events::MouseClickEventArgs &args) override;

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