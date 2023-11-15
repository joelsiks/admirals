#include "GameObject.hpp"

using namespace admirals::scene;

GameObject::GameObject(const Vector3 &pos) : m_position(pos) {}

GameObject::~GameObject() {}

admirals::Vector3 GameObject::position() const {
    Vector3 position = m_position; // A copy
    return position;
}

void GameObject::setPosition(const Vector3 &pos) { this->m_position = pos; }
