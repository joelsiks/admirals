#include "GameObject.hpp"

using namespace admirals::scene;

GameObject::GameObject(const Vector3 &pos) : m_position(pos) {}

GameObject::~GameObject() {}

admirals::Vector3 GameObject::position() const {
    return m_position; // A copy
}

void GameObject::setPosition(const Vector3 &pos) { this->m_position = pos; }
