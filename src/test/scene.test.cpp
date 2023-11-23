#include <Engine.hpp>
#include <chrono>
#include <cmath>
#include <stdbool.h>
#include <stdio.h>

using namespace admirals;

class CellObject : public scene::GameObject {

public:
    CellObject(const std::string &name, const Vector3 &pos)
        : scene::GameObject(name, pos) {}

    void OnStart(Context &c) {}

    void OnUpdate(Context &c) {}

    void Render(const Context &c) const {}
};

int test_add() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("b", pos3);

    auto go1 = scene::GameObject::CreateFromDerived(cell1);
    auto go2 = scene::GameObject::CreateFromDerived(cell2);
    auto go3 = scene::GameObject::CreateFromDerived(cell3);
    scene->AddObject(go1);
    scene->AddObject(go2);
    if (scene->NumObjectsInScene() != 2) {
        return EXIT_FAILURE;
    }
    // try adding duplicate item
    scene->AddObject(go1);
    // try adding new object with the same name as one already existing in the
    // list
    scene->AddObject(go3);
    if (scene->NumObjectsInScene() != 2) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int test_remove() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    CellObject cell1_1 = CellObject("a_1", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    CellObject cell2_1 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("c", pos3);

    auto go1 = scene::GameObject::CreateFromDerived(cell1);
    auto go1_1 = scene::GameObject::CreateFromDerived(cell1_1);
    auto go2 = scene::GameObject::CreateFromDerived(cell2);
    auto go2_1 = scene::GameObject::CreateFromDerived(cell2_1);
    auto go3 = scene::GameObject::CreateFromDerived(cell3);
    scene->AddObject(go1);
    scene->AddObject(go1_1);
    scene->AddObject(go2);
    scene->AddObject(go3);

    std::vector<std::string> str = {"c", "a", "a_1", "b"};
    if (scene->GetSceneObjectNames() != str) {
        return EXIT_FAILURE;
    }
    if (scene->NumObjectsInScene() != 4) {
        return EXIT_FAILURE;
    }
    scene->RemoveObject(go1_1);
    scene->RemoveObject(go1_1);

    str = {"c", "a", "b"};
    if (scene->GetSceneObjectNames() != str) {
        return EXIT_FAILURE;
    }
    if (scene->NumObjectsInScene() != 3) {
        return EXIT_FAILURE;
    }

    scene->RemoveObject(go2_1);
    if (scene->NumObjectsInScene() != 3) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_exist() {

    scene::Scene *scene = new scene::Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    CellObject cell1_1 = CellObject("a", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("c", pos3);

    auto go1 = scene::GameObject::CreateFromDerived(cell1);
    auto go1_1 = scene::GameObject::CreateFromDerived(cell1_1);
    auto go2 = scene::GameObject::CreateFromDerived(cell2);
    scene->AddObject(go1);

    bool exist = scene->ExistObject(go1);
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistObject(go1_1);
    if (exist != false) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistObject(go2);
    if (exist != false) {
        return EXIT_FAILURE;
    }

    exist = scene->ExistObject("a");
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistObject("b");
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
    int index = 0;
    for (int res_value : res) {
        tot_res += res_value;
        if (res_value) {
            std::cout << test_names[index] << " failed\n";
        }
        index++;
    }
    std::cout << "all test done, " << tot_res << " tests failed\n";

    if (tot_res > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}