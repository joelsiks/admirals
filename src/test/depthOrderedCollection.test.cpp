#include "Engine.hpp"
#include "GameObject.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace admirals;

class TestObject : public GameObject {
public:
    TestObject(const std::string &name, float order)
        : GameObject(name, order) {}
    void Render(const EngineContext &c) const override {}
};

class TestInitialSize : public CppUnit::TestCase {
public:
    TestInitialSize() : CppUnit::TestCase("initial size") {}
    void runTest() override {
        const OrderedCollection collection = OrderedCollection<TestObject>();
        const size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not initially 0, was: " +
                                   std::to_string(size),
                               size == 0);
    }
};

class TestInsertSize : public CppUnit::TestCase {
public:
    TestInsertSize() : CppUnit::TestCase("insert size") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<TestObject>();
        const auto t1 = std::make_shared<TestObject>("a", 0);
        collection.Insert(t1);
        const size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 1 after insert, was: " +
                                   std::to_string(size),
                               size == 1);
    }
};

class TestInsertSameName : public CppUnit::TestCase {
public:
    TestInsertSameName() : CppUnit::TestCase("insert same name") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<TestObject>();
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("a", 1);
        collection.Insert(t1);
        collection.Insert(t2);
        const size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 1 after insert, was: " +
                                   std::to_string(size),
                               size == 1);
    }
};

class TestInsertSameZIndex : public CppUnit::TestCase {
public:
    TestInsertSameZIndex() : CppUnit::TestCase("insert same z-index") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<TestObject>();
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 0);
        collection.Insert(t1);
        collection.Insert(t2);
        const size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 2 after insert, was: " +
                                   std::to_string(size),
                               size == 2);
    }
};

class TestInsertMultiple : public CppUnit::TestCase {
public:
    TestInsertMultiple() : CppUnit::TestCase("insert multiple") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<TestObject>();
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 1);
        const auto t3 = std::make_shared<TestObject>("c", 2);
        collection.Insert(t1);
        collection.Insert(t2);
        collection.Insert(t3);
        const size_t size = collection.Size();
        CPPUNIT_ASSERT_MESSAGE("collection size not 3 after insert, was: " +
                                   std::to_string(size),
                               size == 3);
    }
};

class TestOrderedZIndex : public CppUnit::TestCase {
public:
    TestOrderedZIndex() : CppUnit::TestCase("ordered by z-index") {}
    void runTest() override {
        OrderedCollection collection = OrderedCollection<TestObject>();
        const auto t1 = std::make_shared<TestObject>("a", 0);
        const auto t2 = std::make_shared<TestObject>("b", 1);
        const auto t3 = std::make_shared<TestObject>("c", 2);
        collection.Insert(t1);
        collection.Insert(t2);
        collection.Insert(t3);

        float prev = -1;
        for (const auto &o : collection) {
            const float order = o->order();
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
        OrderedCollection collection = OrderedCollection<TestObject>();
        const Vector2 pos = Vector2(2, 4);
        const auto t1 = std::make_shared<TestObject>("a", 0);
        t1->SetPosition(pos);
        collection.Insert(t1);

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