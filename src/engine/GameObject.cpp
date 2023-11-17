#include "GameObject.hpp"

using namespace admirals;
using namespace admirals::scene;

GameObject::GameObject(const std::string &name, float order, const Vector2 &pos)
    : IOrdered(name, order), m_position(pos) {}

GameObject::GameObject(const std::string &name, const Vector3 &pos)
    : GameObject(name, pos.z(), pos.xy()) {}
