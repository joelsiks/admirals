#include "GameObject.hpp"

using namespace admirals;
using namespace admirals::scene;

GameObject::GameObject(const std::string &name, const Vector3 &pos)
    : m_name(name), m_position(pos) {}

std::string GameObject::name() const { return m_name; }
float GameObject::order() const { return m_position.z(); }

GameObject::~GameObject() {}

Vector2 GameObject::position() const {
    return m_position; // A copy
}

void GameObject::setPosition(const Vector2 &pos) { m_position = pos; }
