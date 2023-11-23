#pragma once
#include <memory>

#include "DataObjects.hpp"
#include "IDrawable.hpp"
#include "IOrdered.hpp"

namespace admirals::scene {

class GameObject : public IOrdered, public renderer::IDrawable {
public:
    GameObject(const std::string &name, float order, const Vector2 &position);

    /// @brief This expands to `GameObject(name, position.z(), position.xy())`;
    GameObject(const std::string &name, const Vector3 &position);

    inline Vector2 GetPosition() const { return m_position; }
    inline void SetPosition(const Vector2 &pos) { m_position = pos; }
    inline void SetPosition(float x, float y) { m_position = Vector2(x, y); }

    virtual void OnStart(Context &c) = 0;
    virtual void OnUpdate(Context &c) = 0;

    virtual void Render(const Context &c) const = 0;

    template <typename T>
    static std::shared_ptr<GameObject>
    CreateFromDerived(const T &derivedObject) {
        // Assuming T is derived from GameObject
        std::shared_ptr<GameObject> gameObject =
            std::make_shared<T>(derivedObject);
        return gameObject;
    }

private:
    Vector2 m_position;
};

} // namespace admirals::scene