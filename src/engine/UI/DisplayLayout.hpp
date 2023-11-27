#pragma once

#include <memory>
#include <vector>

#include "InteractiveDrawable.hpp"
#include "OrderedCollection.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public InteractiveDrawable {
public:
    void Render(const EngineContext &c) const override;

    virtual void OnClick(events::MouseClickEventArgs &args) override;

    void AddElement(std::shared_ptr<Element> element);

    static Vector2 GetPositionFromOrientation(DisplayOrientation orientation,
                                              const Vector2 &displaySize,
                                              const EngineContext &r);

protected:
    OrderedCollection<Element> m_elements;
};

} // namespace admirals::UI