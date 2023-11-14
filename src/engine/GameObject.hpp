#pragma once

#include <memory>

#include "Model.hpp"
#include "VK2D/Renderer.h"

namespace admirals {
namespace scene {

class GameObject {
private:
    // Used to sort objects.
    float m_index;

protected:
    vec2 position;

public:
    GameObject(const vec2 &position, float index);
    ~GameObject();

    float index() const;

    virtual void onUpdate() = 0;
    virtual void onStart() = 0;
    virtual void render() = 0; // Should not be part of GameObject...

    template <typename T>
    static std::shared_ptr<GameObject>
    createFromDerived(const T &derivedObject) {
        // Assuming T is derived from GameObject
        std::shared_ptr<GameObject> gameObject =
            std::make_shared<T>(derivedObject);
        return gameObject;
    }
};

} // namespace scene
} // namespace admirals