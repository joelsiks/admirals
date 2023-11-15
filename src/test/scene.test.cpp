#include <Engine.hpp>
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>
#include <chrono>
#include <cmath>
#include <stdbool.h>
#include <stdio.h>

#include <memory>
#include <thread>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
float deltaT = 0;

using namespace admirals;

class CellObject : public scene::GameObject {
private:
    vec4 color;

public:
    CellObject(const vec2 &pos, const float &index, const vec4 &color)
        : scene::GameObject(pos, index) {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->color[3] = color[3];
    }

    void onStart() {}

    void onUpdate() {
        this->position[0] = (this->position[0] + 1000.f * deltaT);
        if (this->position[0] > WINDOW_WIDTH) {
            this->position[0] = -100 + this->position[0] - WINDOW_WIDTH;
        }
    }

    void render() {
        vec2 size = {100, 100};
        renderer::Renderer::drawRectangle(this->position, size, this->color);
    }
};

int test_add() {

    scene::Scene *scene = new scene::Scene();

    vec2 pos1 = {0, 0};
    CellObject cell1 = CellObject(pos1, 2, VK2D_BLUE);
    vec2 pos2 = {50, 50};
    CellObject cell2 = CellObject(pos2, 3, VK2D_RED);
    vec2 pos3 = {100, 100};
    CellObject cell3 = CellObject(pos3, 1, VK2D_BLACK);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go1_1 = scene::GameObject::createFromDerived(cell1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    auto go3 = scene::GameObject::createFromDerived(cell3);
    scene->setDupsAllowed(false);
    scene->addObject(go1);
    scene->addObject(go2);
    scene->addObject(go3);

    if (scene->sizeObjects() != 3) {
        return EXIT_FAILURE;
    }

    scene->addObject(go1);
    if (scene->sizeObjects() != 3) {
        return EXIT_FAILURE;
    }
    scene->addObject(go1_1);
    if (scene->sizeObjects() != 3) {
        return EXIT_FAILURE;
    }

    scene->setDupsAllowed(true);

    scene->addObject(go1);
    if (scene->sizeObjects() != 4) {
        return EXIT_FAILURE;
    }

    scene->addObject(go1_1);
    if (scene->sizeObjects() != 5) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int test_remove() {

    scene::Scene *scene = new scene::Scene();

    vec2 pos1 = {0, 0};
    CellObject cell1 = CellObject(pos1, 2, VK2D_BLUE);
    vec2 pos2 = {50, 50};
    CellObject cell2 = CellObject(pos2, 3, VK2D_RED);
    vec2 pos3 = {100, 100};
    CellObject cell3 = CellObject(pos3, 1, VK2D_BLACK);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go1_1 = scene::GameObject::createFromDerived(cell1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    auto go3 = scene::GameObject::createFromDerived(cell3);
    scene->setDupsAllowed(true);
    scene->addObject(go1);
    scene->addObject(go1);
    scene->addObject(go1);
    scene->addObject(go1);
    scene->addObject(go1);
    scene->addObject(go2);
    scene->addObject(go3);

    if (scene->sizeObjects() != 7) {
        return EXIT_FAILURE;
    }

    scene->removeOneObject(go1);
    if (scene->sizeObjects() != 6) {
        return EXIT_FAILURE;
    }

    scene->removeAllObject(go1);
    if (scene->sizeObjects() != 2) {
        return EXIT_FAILURE;
    }

    scene->addObject(go1);
    scene->addObject(go1);
    scene->addObject(go1_1);
    scene->addObject(go1_1);
    if (scene->sizeObjects() != 6) {
        return EXIT_FAILURE;
    }

    scene->removeAllObject(go1);
    if (scene->sizeObjects() != 2) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int test_exist() {

    scene::Scene *scene = new scene::Scene();

    vec2 pos1 = {0, 0};
    CellObject cell1 = CellObject(pos1, 2, VK2D_BLUE);
    vec2 pos2 = {50, 50};
    CellObject cell2 = CellObject(pos2, 3, VK2D_RED);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go1_1 = scene::GameObject::createFromDerived(cell1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    scene->addObject(go1);
    scene->setDupsAllowed(false);

    bool exist = scene->existObject(go1);
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->existObject(go1_1);
    if (exist != true) {
        return EXIT_FAILURE;
    }

    exist = scene->existObject(go2);
    if (exist != false) {
        return EXIT_FAILURE;
    }

    scene->removeOneObject(go1);
    exist = scene->existObject(go1);
    if (exist != false) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    int res = test_add();
    if (res) {
        printf("test_add FAILURE\n");
        return EXIT_FAILURE;
    }
    printf("test_add SUCCESS\n");

    res = test_remove();
    if (res) {
        printf("test_remove FAILURE\n");
        return EXIT_FAILURE;
    }
    printf("test_remove SUCCESS\n");

    res = test_exist();
    if (res) {
        printf("test_exist FAILURE\n");
        return EXIT_FAILURE;
    }
    printf("test_exist SUCCESS\n");

    printf("all complete\n");
    return EXIT_SUCCESS;
}