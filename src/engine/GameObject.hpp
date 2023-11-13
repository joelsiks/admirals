#pragma once
#include "Model.hpp"
namespace engine 
{

/// \brief 2D vector of floats
typedef float vec2[2];

/// \brief 2D vector of integers
typedef int location_t[2];

class GameObject
{
private:
    /* data */
    Model model;
    location_t position;

public:
    GameObject();
    ~GameObject();
    
    void OnUpdate();
    void OnStart();
};

GameObject::GameObject()
{
    
}

GameObject::~GameObject()
{
    
}

}