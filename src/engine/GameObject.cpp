#include "GameObject.hpp"

using namespace admirals;
using namespace admirals::scene;

GameObject::GameObject(const std::string &name, const Vector3 &pos)
    : IOrderedObject(name, pos.z()), m_position(pos) {}

GameObject::~GameObject() {}

Vector2 GameObject::position() const {
    return m_position; // A copy
}

void GameObject::setPosition(const Vector2 &pos) { m_position = pos; }
