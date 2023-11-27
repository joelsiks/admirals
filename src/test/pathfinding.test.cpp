
#include "Pathfinding.cpp"

using namespace admirals;

class CellObject : public scene::GameObject {

public:
    CellObject(const std::string &name, const Vector3 &pos)
        : scene::GameObject(name, pos) {}
    void OnStart(const EngineContext &c) {}
    void OnUpdate(const EngineContext &c) {}
    void Render(const EngineContext &c) const {}
};

int main(int argc, char *argv[]) {

    Vector2 size = Vector2(10, 10);
    OrderedCollection<GameObject> orderedList = OrderedCollection<GameObject>();

    auto obj1 =
        scene::GameObject::CreateFromDerived(CellObject("a", {2, 2, 2}));
    orderedList.Insert(std::move(obj1));

    Pathfinding pf = Pathfinding();
    pf.visualizeGrid(size, orderedList);
    Vector2 vec = {5, 7};
    std::cout << vec.x() << "--" << vec.y();

    pf.findpath(size, orderedList, obj1, vec);

    return EXIT_SUCCESS;
}