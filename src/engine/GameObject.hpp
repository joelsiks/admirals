#pragma once

#include "DataObjects.hpp"
#include <memory>

namespace admirals::scene {

class GameObject {

public:
    GameObject(const Vector3 &position);
    ~GameObject();

    Vector3 position() const;

    virtual void onUpdate() = 0;
    virtual void onStart() = 0;
    virtual void render() const = 0; // Should not be part of GameObject...

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

protected:
    void setPosition(const Vector3 &pos);
};

} // namespace admirals::scene