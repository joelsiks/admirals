#include "Scene.hpp"

namespace admirals {
namespace scene {

void Scene::render() const {
    for (auto &object : this->objects) {
        object->onUpdate();
        object->render();
    }
}

/**
 * recieves a pointer to a gamObject and insert it to the multiset.
 * If the 'dupsAllowed' varaible is true multiple pointers to the same object can be stored.
 * otherwise only one pointer is allowed per object.
*/
void Scene::addObject(std::shared_ptr<GameObject> object) {
    if ((!this->dupsAllowed) && existObject(object)) {
        return;
    }
    this->objects.insert(object);
}

/**
 * recieves a pointer to a gameObject and removes all from the multiset
*/
void Scene::removeAllObject(std::shared_ptr<GameObject> object) {
    this->objects.erase(object);
}

/**
 * recieves a pointer to a gameObject and deletes the first instance it sees.
 * Will not remove all pointers if multiple are present
*/
void Scene::removeOneObject(std::shared_ptr<GameObject> object) {
    this->objects.erase(this->objects.find(object));
}

/**
 * recieves a pointer to a gameObject and checks if it exist in the multiset
*/
bool Scene::existObject(std::shared_ptr<GameObject> object) {
    return (this->objects.find(object) != this->objects.end());
}

/**
 * returns the total amount of pointers to objects currently in the multiset
*/
int Scene::sizeObjects() {
    return this->objects.size();
}

/**
 * set if the multiset will allow multiple of the same pointer for an object.
 * If set to 'false' will not apply retroactivly.
*/
void Scene::setDupsAllowed(bool statement) {
    this->dupsAllowed = statement;
}



Scene::Scene() { 
    this->objects = {}; 
    this->gridLenght = 20;
    this->gridHeight = 20;
    this->dupsAllowed = false;
}

Scene::~Scene() {}

} // namespace scene
} // namespace admirals