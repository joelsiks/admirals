
#include "Engine.hpp"
#include "GameObject.hpp"
#include "TestHandler.hpp"

using namespace admirals;
using namespace admirals::test;

class TestObject : public scene::GameObject {
public:
    TestObject(const std::string &name, float order)
        : scene::GameObject(name, order) {}
    void Render(const EngineContext &c) const override {}
};

class TestInitialSize : public TestCaseBase {
public:
    TestInitialSize() : TestCaseBase("initial size") {}
    void test() override {
        const OrderedCollection collection = OrderedCollection();
        const size_t size = collection.Size();
        assert(size == 0,
               "collection size not initially 0, was: " + std::to_string(size));
    }
};

class TestInsertSize : public TestCaseBase {
public:
    TestInsertSize() : TestCaseBase("insert size") {}
    void test() override {
        OrderedCollection collection = OrderedCollection();
        const TestObject t1 = TestObject("a", 0);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        const size_t size = collection.Size();
        assert(size == 1, "collection size not 1 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertSameName : public TestCaseBase {
public:
    TestInsertSameName() : TestCaseBase("insert same name") {}
    void test() override {
        OrderedCollection collection = OrderedCollection();
        const TestObject t1 = TestObject("a", 0);
        const TestObject t2 = TestObject("a", 1);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        const size_t size = collection.Size();
        assert(size == 1, "collection size not 1 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertSameZIndex : public TestCaseBase {
public:
    TestInsertSameZIndex() : TestCaseBase("insert same z-index") {}
    void test() override {
        OrderedCollection collection = OrderedCollection();
        const TestObject t1 = TestObject("a", 0);
        const TestObject t2 = TestObject("b", 0);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        const size_t size = collection.Size();
        assert(size == 2, "collection size not 2 after insert, was: " +
                              std::to_string(size));
    }
};

class TestInsertMultiple : public TestCaseBase {
public:
    TestInsertMultiple() : TestCaseBase("insert multiple") {}
    void test() override {
        OrderedCollection collection = OrderedCollection();
        const TestObject t1 = TestObject("a", 0);
        const TestObject t2 = TestObject("b", 1);
        const TestObject t3 = TestObject("c", 2);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        collection.Insert(scene::GameObject::CreateFromDerived(t3));
        const size_t size = collection.Size();
        assert(size == 3, "collection size not 3 after insert, was: " +
                              std::to_string(size));
    }
};

class TestOrderedZIndex : public TestCaseBase {
public:
    TestOrderedZIndex() : TestCaseBase("ordered by z-index") {}
    void test() override {
        OrderedCollection collection = OrderedCollection();
        const TestObject t1 = TestObject("a", 0);
        const TestObject t2 = TestObject("b", 1);
        const TestObject t3 = TestObject("c", 2);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        collection.Insert(scene::GameObject::CreateFromDerived(t3));

        float prev = -1;
        for (const auto &o : collection) {
            const float order = o->order();
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
        OrderedCollection collection = OrderedCollection<scene::GameObject>();
        const Vector2 pos = Vector2(2, 4);
        TestObject t1 = TestObject("a", 0);
        t1.SetPosition(pos);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));

        for (const auto &o : collection) {
            const Vector2 p = o->GetPosition();
            assert(pos == p, "object position not as expected");
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