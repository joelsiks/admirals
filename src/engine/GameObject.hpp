#pragma once
#include <memory>

#include "DataObjects.hpp"
#include "IDrawable.hpp"
#include "IOrderedObject.hpp"

namespace admirals {
namespace scene {

class GameObject : public IOrderedObject {

public:
    GameObject(const std::string &name, const Vector3 &position);
    ~GameObject();

    Vector2 position() const;
    void setPosition(const Vector2 &pos);

    virtual void onUpdate() = 0;
    virtual void onStart() = 0;
    // Should not be part of GameObject...
    virtual void render(const renderer::RendererContext &r) = 0;

    template <typename T>
    static std::shared_ptr<GameObject>
    createFromDerived(const T &derivedObject) {
        // Assuming T is derived from GameObject
        std::shared_ptr<GameObject> gameObject =
            std::make_shared<T>(derivedObject);
        return gameObject;
    }

private:
    Vector3 m_position;
};

} // namespace scene
} // namespace admirals