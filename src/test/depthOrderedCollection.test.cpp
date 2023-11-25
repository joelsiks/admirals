#include "Engine.hpp"
#include "GameObject.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace admirals;

class TestObject : public scene::GameObject {
public:
    TestObject(const std::string &name, float order, const Vector2 &pos)
        : scene::GameObject(name, order, pos) {}
    void OnStart() {}
    void OnUpdate() {}
    void Render(const renderer::RendererContext &r) const {}
};

class TestInitialSize : public CppUnit::TestCase {
public:
    TestInitialSize() : CppUnit::TestCase("initial size") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not initially 0, was: " +
                                   std::to_string(size),
                               size == 0);
    }
};

class TestInsertSize : public CppUnit::TestCase {
public:
    TestInsertSize() : CppUnit::TestCase("insert size") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        TestObject t1 = TestObject("a", 0, Vector2(0, 0));
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 1 after insert, was: " +
                                   std::to_string(size),
                               size == 1);
    }
};

class TestInsertSameName : public CppUnit::TestCase {
public:
    TestInsertSameName() : CppUnit::TestCase("insert same name") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        TestObject t1 = TestObject("a", 0, Vector2(0, 0));
        TestObject t2 = TestObject("a", 1, Vector2(1, 1));
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 1 after insert, was: " +
                                   std::to_string(size),
                               size == 1);
    }
};

class TestInsertSameZIndex : public CppUnit::TestCase {
public:
    TestInsertSameZIndex() : CppUnit::TestCase("insert same z-index") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        TestObject t1 = TestObject("a", 0, Vector2(0, 0));
        TestObject t2 = TestObject("b", 0, Vector2(1, 1));
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 2 after insert, was: " +
                                   std::to_string(size),
                               size == 2);
    }
};

class TestInsertMultiple : public CppUnit::TestCase {
public:
    TestInsertMultiple() : CppUnit::TestCase("insert multiple") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        TestObject t1 = TestObject("a", 0, Vector2(0, 0));
        TestObject t2 = TestObject("b", 1, Vector2(1, 1));
        TestObject t3 = TestObject("c", 2, Vector2(2, 2));
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        collection.Insert(scene::GameObject::CreateFromDerived(t3));
        size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 3 after insert, was: " +
                                   std::to_string(size),
                               size == 3);
    }
};

class TestOrderedZIndex : public CppUnit::TestCase {
public:
    TestOrderedZIndex() : CppUnit::TestCase("ordered by z-index") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection();
        TestObject t1 = TestObject("a", 0, Vector2(0, 0));
        TestObject t2 = TestObject("b", 1, Vector2(1, 1));
        TestObject t3 = TestObject("c", 2, Vector2(2, 2));
        collection.Insert(scene::GameObject::CreateFromDerived(t1));
        collection.Insert(scene::GameObject::CreateFromDerived(t2));
        collection.Insert(scene::GameObject::CreateFromDerived(t3));

        float prev = -1;
        for (const auto &o : collection) {
            float order = o->order();
            CPPUNIT_ASSERT_MESSAGE(
                "object out of order, index " + std::to_string(order) +
                    " appeared after " + std::to_string(prev),
                prev <= order);
            prev = order;
        }
    }
};

class TestTypedObjects : public CppUnit::TestCase {
public:
    TestTypedObjects() : CppUnit::TestCase("typed objects") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<scene::GameObject>();
        Vector2 pos = Vector2(0);
        TestObject t1 = TestObject("a", 0, pos);
        collection.Insert(scene::GameObject::CreateFromDerived(t1));

        for (const auto &o : collection) {
            const Vector2 p = o->GetPosition();
            CPPUNIT_ASSERT_MESSAGE("object position not as expected", pos == p);
        }
    }
};

int main(int argc, char *argv[]) {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(new TestInitialSize());
    runner.addTest(new TestInsertSize());
    runner.addTest(new TestInsertSameName());
    runner.addTest(new TestInsertSameZIndex());
    runner.addTest(new TestInsertMultiple());
    runner.addTest(new TestOrderedZIndex());
    runner.addTest(new TestTypedObjects());
    runner.run();
    return EXIT_SUCCESS;
}