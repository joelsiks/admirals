#include <Engine.hpp>
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>
#include <chrono>
#include <cmath>
#include <stdbool.h>
#include <stdio.h>

using namespace admirals;

class CellObject : public scene::GameObject {

public:
    CellObject(const Vector3 &pos)
        : scene::GameObject(pos) {}

    void onStart() {}

    void onUpdate() {}

    void render(const renderer::RendererContext &r) {}
};

int test_add() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject(pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject(pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject(pos3);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    auto go3 = scene::GameObject::createFromDerived(cell3);
    scene->addObject(go1);
    scene->addObject(go2);
    scene->addObject(go3);

    if (scene->numObjectsInScene() != 3) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_remove() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject(pos1);
    CellObject cell1_1 = CellObject(pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject(pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject(pos3);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go1_1 = scene::GameObject::createFromDerived(cell1_1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    auto go3 = scene::GameObject::createFromDerived(cell3);
    scene->addObject(go1);
    scene->addObject(go1_1);
    scene->addObject(go1);
    scene->addObject(go2);
    scene->addObject(go3);

    if (scene->numObjectsInScene() != 5) {
        return EXIT_FAILURE;
    }
    scene->removeObject(go1_1);
    scene->removeObject(go1_1);
    if (scene->numObjectsInScene() != 4) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_exist() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject(pos1);
    CellObject cell1_1 = CellObject(pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject(pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject(pos3);

    auto go1 = scene::GameObject::createFromDerived(cell1);
    auto go1_1 = scene::GameObject::createFromDerived(cell1_1);
    auto go2 = scene::GameObject::createFromDerived(cell2);
    scene->addObject(go1);

    bool exist = scene->existObject(go1);
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->existObject(go1_1);
    if (exist != false) {
        return EXIT_FAILURE;
    }
    exist = scene->existObject(go2);
    if (exist != false) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    int res[3];
    const char *test_names[3] = {"test_add", "test_remove", "test_exist"};

    int tot_res = 0;

    res[0] = test_add();
    res[1] = test_remove();
    res[2] = test_exist();
    for (int res_value : res) {
        tot_res += res_value;
        if (res_value) {
            printf("%d failed\n", test_names[res_value]);
        }
    }
    printf("all test done, %d tests failed\n", tot_res);

    if (tot_res > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}