#pragma once
#include "Model.hpp"
#include "VK2D/Renderer.h"

namespace admirals {
namespace scene {

class GameObject
{
private:
    float _index;

protected:
    vec2 position;

public:
    GameObject(const vec2& position, const float& index);
    ~GameObject();

    float index() const;

    virtual void onUpdate() = 0;
    virtual void onStart() = 0;
    virtual void render() = 0; // Should not be part of GameObject...
};

}}