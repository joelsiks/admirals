#pragma once
#include <memory>

#include "DataObjects.hpp"
#include "IDrawable.hpp"
#include "IOrdered.hpp"

namespace admirals {
namespace scene {

class GameObject : public IOrdered {

public:
    GameObject(const std::string &name, const Vector3 &position);
    ~GameObject();

    Vector2 position() const;
    void setPosition(const Vector2 &pos);

    std::string name() const;
    float order() const;

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
    const std::string m_name;
};

} // namespace scene
} // namespace admirals