#include "GameObject.hpp"

namespace admirals {
namespace scene {

GameObject::GameObject(const vec2 &pos, float index) {
    this->position[0] = pos[0];
    this->position[1] = pos[1];
    this->m_index = index;
}

GameObject::~GameObject() {}

float GameObject::index() const { return this->m_index; }

} // namespace scene
} // namespace admirals