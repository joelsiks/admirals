#include "GameObject.hpp"

namespace admirals {
namespace scene {

GameObject::GameObject(const vec2 &pos, const float &index) {
    this->position[0] = pos[0];
    this->position[1] = pos[1];
    this->_index = index;
}

GameObject::~GameObject() {}

float GameObject::index() const { return this->_index; }

} // namespace scene
} // namespace admirals