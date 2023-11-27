#pragma once
#include <memory>

#include "DataObjects.hpp"
#include "IDisplayable.hpp"
#include "IInteractiveDrawable.hpp"
#include "IOrdered.hpp"

namespace admirals::scene {

class GameObject : public IOrdered,
                   public IInteractiveDrawable,
                   public IDisplayable {
public:
    GameObject(const std::string &name, float order, const Vector2 &position,
               const Vector2 &size)
        : GameObject(name, order, Rect(position, size)) {}

    GameObject(const std::string &name, float order = 0.f,
               const Rect &bounds = Rect())
        : IOrdered(name, order), IDisplayable(bounds) {}

    // Engine event handlers
    virtual void OnStart(const EngineContext &ctx) {}
    virtual void OnUpdate(const EngineContext &ctx) {}
    virtual void OnClick(events::MouseClickEventArgs &args) {}
    virtual void OnMouseEnter(events::EventArgs &args) {}
    virtual void OnMouseLeave(events::EventArgs &args) {}
    virtual void OnMouseMove(events::EventArgs &args) {}

    virtual void Render(const EngineContext &ctx) const = 0;

    template <typename T>
    static std::shared_ptr<GameObject>
    CreateFromDerived(const T &derivedObject) {
        // Assuming T is derived from GameObject
        std::shared_ptr<GameObject> gameObject =
            std::make_shared<T>(derivedObject);
        return gameObject;
    }
};

} // namespace admirals::scene