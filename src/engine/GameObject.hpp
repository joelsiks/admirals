#pragma once
#include <memory>

#include "DataObjects.hpp"
#include "IDisplayable.hpp"
#include "IInteractiveDisplayable.hpp"
#include "IOrdered.hpp"

namespace admirals {

class GameObject : public IInteractiveDisplayable {
public:
    GameObject(const std::string &name, float order, const Vector2 &position,
               const Vector2 &size)
        : GameObject(name, order, Rect(position, size)) {}

    GameObject(const std::string &name, float order = 0.f,
               const Rect &bounds = Rect())
        : IInteractiveDisplayable(name, order, bounds) {}

    // Engine event handlers
    virtual void OnStart(const EngineContext &ctx) {}
    virtual void OnUpdate(const EngineContext &ctx) {}

    virtual void OnClick(events::MouseClickEventArgs &args) override {}

    virtual void OnMouseEnter(events::MouseMotionEventArgs &args) override {}
    virtual void OnMouseLeave(events::MouseMotionEventArgs &args) override {}
    virtual void OnMouseMove(events::MouseMotionEventArgs &args) override {}

    virtual void OnShown() override {}
    virtual void OnHidden() override {}

    virtual void Render(const EngineContext &ctx) const = 0;
};

} // namespace admirals