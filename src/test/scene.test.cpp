#include <Engine.hpp>
#include <chrono>
#include <cmath>
#include <stdbool.h>
#include <stdio.h>

using namespace admirals;

class CellObject : public GameObject {

public:
    CellObject(const std::string &name, const Vector3 &pos)
        : GameObject(name, pos.z(), pos.xy(), Vector2()) {}

    void Render(const EngineContext &c) const {}
};

int test_add() {

    Scene *scene = new Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("b", pos3);

    auto go1 = GameObject::CreateFromDerived(cell1);
    auto go2 = GameObject::CreateFromDerived(cell2);
    auto go3 = GameObject::CreateFromDerived(cell3);
    scene->AddDisplayable(go1);
    scene->AddDisplayable(go2);
    if (scene->NumDisplayables() != 2) {
        return EXIT_FAILURE;
    }
    // try adding duplicate item
    scene->AddDisplayable(go1);
    // try adding new object with the same name as one already existing in the
    // list
    scene->AddDisplayable(go3);
    if (scene->NumDisplayables() != 2) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int test_remove() {

    Scene *scene = new Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    CellObject cell1_1 = CellObject("a_1", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    CellObject cell2_1 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("c", pos3);

    auto go1 = GameObject::CreateFromDerived(cell1);
    auto go1_1 = GameObject::CreateFromDerived(cell1_1);
    auto go2 = GameObject::CreateFromDerived(cell2);
    auto go2_1 = GameObject::CreateFromDerived(cell2_1);
    auto go3 = GameObject::CreateFromDerived(cell3);
    scene->AddDisplayable(go1);
    scene->AddDisplayable(go1_1);
    scene->AddDisplayable(go2);
    scene->AddDisplayable(go3);

    std::vector<std::string> str = {"c", "a", "a_1", "b"};
    if (scene->GetDisplayableNames() != str) {
        return EXIT_FAILURE;
    }
    if (scene->NumDisplayables() != 4) {
        return EXIT_FAILURE;
    }
    scene->RemoveDisplayable(go1_1->identifier());
    scene->RemoveDisplayable(go1_1->identifier());

    str = {"c", "a", "b"};
    if (scene->GetDisplayableNames() != str) {
        return EXIT_FAILURE;
    }
    if (scene->NumDisplayables() != 3) {
        return EXIT_FAILURE;
    }

    scene->RemoveDisplayable(go2_1->identifier());
    if (scene->NumDisplayables() != 3) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_exist() {

    Scene *scene = new Scene();

    Vector3 pos1 = {0, 0, 2};
    CellObject cell1 = CellObject("a", pos1);
    CellObject cell1_1 = CellObject("a", pos1);
    Vector3 pos2 = {50, 50, 3};
    CellObject cell2 = CellObject("b", pos2);
    Vector3 pos3 = {100, 100, 1};
    CellObject cell3 = CellObject("c", pos3);

    auto go1 = GameObject::CreateFromDerived(cell1);
    auto go1_1 = GameObject::CreateFromDerived(cell1_1);
    auto go2 = GameObject::CreateFromDerived(cell2);
    scene->AddDisplayable(go1);

    bool exist = scene->ExistsDisplayable(go1->identifier());
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistsDisplayable(go1_1->identifier());
    if (exist != false) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistsDisplayable(go2->identifier());
    if (exist != false) {
        return EXIT_FAILURE;
    }

    exist = scene->ExistsDisplayable("a");
    if (exist != true) {
        return EXIT_FAILURE;
    }
    exist = scene->ExistsDisplayable("b");
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