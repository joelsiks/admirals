#pragma once
#include <list>
#include <vector>
#include "GameObject.hpp"
namespace engine 
{
class Scene
{
private:
    /* data */
    std::list<GameObject> objects;
public:
    std::vector<GameObject> GetObjects();

    void AddObject(const GameObject object);

    Scene();
    ~Scene();
};

std::vector<GameObject> Scene::GetObjects()
{
    std::vector<GameObject> v { objects.begin(), objects.end() };
    return v;
}

void Scene::AddObject(const GameObject object)
{
    this->objects.push_back(object);
}

Scene::Scene()
{
    this->objects = {};
}

Scene::~Scene()
{

}

}