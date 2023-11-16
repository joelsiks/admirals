
#include "Engine.hpp"
#include "GameObject.hpp"
#include "TestHandler.hpp"

using namespace admirals;
using namespace admirals::test;

class TestObject : public scene::GameObject {
public:
    TestObject(const std::string &name, const Vector3 &pos)
        : scene::GameObject(name, pos) {}
    void onStart() {}
    void onUpdate() {}
    void render(const renderer::RendererContext &r) {}
};

class TestInitialSize : public TestCaseBase {
public:
    TestInitialSize() : TestCaseBase("initial size") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        size_t size = collection.size();
        assert(size == 0,
               "collection size not initially 0, was: " + std::to_string(size));
    }
};

class TestInsertSize : public TestCaseBase {
public:
    TestInsertSize() : TestCaseBase("insert size") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        TestObject t1 = TestObject("a", Vector3(0, 0, 0));
        collection.insert(scene::GameObject::createFromDerived(t1));
        size_t size = collection.size();
        assert(size == 1, "collection size not 1 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertSameName : public TestCaseBase {
public:
    TestInsertSameName() : TestCaseBase("insert same name") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        TestObject t1 = TestObject("a", Vector3(0, 0, 0));
        TestObject t2 = TestObject("a", Vector3(1, 1, 1));
        collection.insert(scene::GameObject::createFromDerived(t1));
        collection.insert(scene::GameObject::createFromDerived(t2));
        size_t size = collection.size();
        assert(size == 1, "collection size not 1 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertSameZIndex : public TestCaseBase {
public:
    TestInsertSameZIndex() : TestCaseBase("insert same z-index") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        TestObject t1 = TestObject("a", Vector3(0, 0, 0));
        TestObject t2 = TestObject("b", Vector3(1, 1, 0));
        collection.insert(scene::GameObject::createFromDerived(t1));
        collection.insert(scene::GameObject::createFromDerived(t2));
        size_t size = collection.size();
        assert(size == 2, "collection size not 2 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertMultiple : public TestCaseBase {
public:
    TestInsertMultiple() : TestCaseBase("insert multiple") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        TestObject t1 = TestObject("a", Vector3(0));
        TestObject t2 = TestObject("b", Vector3(1));
        TestObject t3 = TestObject("c", Vector3(2));
        collection.insert(scene::GameObject::createFromDerived(t1));
        collection.insert(scene::GameObject::createFromDerived(t2));
        collection.insert(scene::GameObject::createFromDerived(t3));
        size_t size = collection.size();
        assert(size == 3, "collection size not 3 after insert, was: " +
                              std::to_string(size));
    }
};

class TestOrderedZIndex : public TestCaseBase {
public:
    TestOrderedZIndex() : TestCaseBase("ordered by z-index") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection();
        TestObject t1 = TestObject("a", Vector3(2));
        TestObject t2 = TestObject("b", Vector3(0));
        TestObject t3 = TestObject("c", Vector3(1));
        collection.insert(scene::GameObject::createFromDerived(t1));
        collection.insert(scene::GameObject::createFromDerived(t2));
        collection.insert(scene::GameObject::createFromDerived(t3));

        float prev = -1;
        for (const auto &o : collection) {
            float order = o->m_order;
            assert(prev <= order,
                   "object out of order, index " + std::to_string(order) +
                       " appeared after " + std::to_string(prev));
            prev = order;
        }
    }
};

class TestTypedObjects : public TestCaseBase {
public:
    TestTypedObjects() : TestCaseBase("typed objects") {}
    void test() override {
        DepthOrderedCollection collection = DepthOrderedCollection<scene::GameObject>();
        Vector3 pos = Vector3(0);
        TestObject t1 = TestObject("a", pos);
        collection.insert(scene::GameObject::createFromDerived(t1));

        for (const auto &o : collection) {
            const Vector2 p = o->position();
            assert(pos.xy() == p, "object position not as expected");
        }
    }
};

int main(int argc, char *argv[]) {
    TestHandler handler = TestHandler();
    handler.add(new TestInitialSize());
    handler.add(new TestInsertSize());
    handler.add(new TestInsertSameName());
    handler.add(new TestInsertSameZIndex());
    handler.add(new TestInsertMultiple());
    handler.add(new TestOrderedZIndex());
    handler.add(new TestTypedObjects());
    handler.run();
}