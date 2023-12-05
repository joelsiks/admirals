
#include "Engine.hpp"
#include "GameObject.hpp"
#include "TestHandler.hpp"

using namespace admirals;
using namespace admirals::test;

class TestObject : public GameObject {
public:
    TestObject(const std::string &name, float order)
        : GameObject(name, order) {}
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
        const auto t1 = std::make_shared<TestObject>("a", 0);
        collection.Insert(t1);
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
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("a", 1);
        collection.Insert(t1);
        collection.Insert(t2);
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
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 0);
        collection.Insert(t1);
        collection.Insert(t2);
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
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 1);
        const auto t3 = std::make_shared<TestObject>("c", 2);
        collection.Insert(t1);
        collection.Insert(t2);
        collection.Insert(t3);
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
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 1);
        const auto t3 = std::make_shared<TestObject>("c", 2);
        collection.Insert(t1);
        collection.Insert(t2);
        collection.Insert(t3);

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
        OrderedCollection collection = OrderedCollection<GameObject>();
        const Vector2 pos = Vector2(2, 4);
        const auto t1 = std::make_shared<TestObject>("a", 0);
        t1->SetPosition(pos);
        collection.Insert(t1);

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