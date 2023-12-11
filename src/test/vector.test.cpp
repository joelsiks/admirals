#include "DataObjects.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace admirals;

class TestCreateVector2 : public CppUnit::TestCase {
public:
    TestCreateVector2() : CppUnit::TestCase("create vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(2.f);
        CPPUNIT_ASSERT(v1.x() == 2.f);
        CPPUNIT_ASSERT(v1.y() == 2.f);

        Vector2 v2 = Vector2(4.f, 6.f);
        CPPUNIT_ASSERT(v2.x() == 4.f);
        CPPUNIT_ASSERT(v2.y() == 6.f);
    }
};

class TestAccessPointerVector2 : public CppUnit::TestCase {
public:
    TestAccessPointerVector2()
        : CppUnit::TestCase("access data pointer of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(2.f);
        CPPUNIT_ASSERT(v1.Data()[0] == 2.f);
        CPPUNIT_ASSERT(v1.Data()[1] == 2.f);

        Vector2 v2 = Vector2(4.f, 6.f);
        CPPUNIT_ASSERT(v2.Data()[0] == 4.f);
        CPPUNIT_ASSERT(v2.Data()[1] == 6.f);
    }
};

class TestSetVector2 : public CppUnit::TestCase {
public:
    TestSetVector2() : CppUnit::TestCase("set values of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(2.f);
        v1.SetX(4.f);
        CPPUNIT_ASSERT(v1.x() == 4.f);
        CPPUNIT_ASSERT(v1.y() == 2.f);

        Vector2 v2 = Vector2(2.f);
        v2.SetY(4.f);
        CPPUNIT_ASSERT(v2.x() == 2.f);
        CPPUNIT_ASSERT(v2.y() == 4.f);
    }
};

class TestEqualsVector2 : public CppUnit::TestCase {
public:
    TestEqualsVector2() : CppUnit::TestCase("equals of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        Vector2 v3 = Vector2(2.f, 2.f);
        CPPUNIT_ASSERT(v1 != v2);
        CPPUNIT_ASSERT(v2 == v3);
    }
};

class TestAssignmentVector2 : public CppUnit::TestCase {
public:
    TestAssignmentVector2() : CppUnit::TestCase("assignment of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        v1 = v2;
        CPPUNIT_ASSERT(v1 == v2);
        v2.SetX(0);
        CPPUNIT_ASSERT(v1 != v2);
    }
};

class TestAdditionVector2 : public CppUnit::TestCase {
public:
    TestAdditionVector2() : CppUnit::TestCase("vector addition of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        Vector2 v3 = Vector2(3.f);
        Vector2 v4 = Vector2(2.f, 1.f);
        CPPUNIT_ASSERT(v1 + v2 == v3);
        CPPUNIT_ASSERT(v1 + v1 != v3);
        v2 += v1;
        CPPUNIT_ASSERT(v2 == v3);
        CPPUNIT_ASSERT(v1 + v1 + v1 == v2);
        CPPUNIT_ASSERT(v4 + v2 == Vector2(5.f, 4.f));
        CPPUNIT_ASSERT(v4 + v3 == v3 + v4);
    }
};

class TestSubtractionVector2 : public CppUnit::TestCase {
public:
    TestSubtractionVector2()
        : CppUnit::TestCase("vector subtraction of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        Vector2 v3 = Vector2(3.f);
        Vector2 v4 = Vector2(2.f, 1.f);
        CPPUNIT_ASSERT(v3 - v1 == v2);
        CPPUNIT_ASSERT(v1 - v2 != v3);
        v3 -= v1;
        CPPUNIT_ASSERT(v2 == v3);
        CPPUNIT_ASSERT(v3 - v1 - v1 == v1);
        CPPUNIT_ASSERT(v4 - v4 == Vector2(0));
        CPPUNIT_ASSERT(v4 - v2 == Vector2(0, -1.f));
        CPPUNIT_ASSERT(v4 - v2 != v2 - v4);
    }
};

class TestVectorMultiplicationVector2 : public CppUnit::TestCase {
public:
    TestVectorMultiplicationVector2()
        : CppUnit::TestCase("vector multiplication of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        Vector2 v3 = Vector2(3.f);
        Vector2 v4 = Vector2(4.f);
        Vector2 v6 = Vector2(6.f);
        Vector2 v7 = Vector2(1.f, 2.f);
        CPPUNIT_ASSERT(v1 * v1 == v1);
        CPPUNIT_ASSERT(v1 * v3 == v3);
        CPPUNIT_ASSERT(v2 * v2 == v4);
        v3 *= v2;
        CPPUNIT_ASSERT(v3 == v6);
        CPPUNIT_ASSERT(v2 == v1 + v1);
        CPPUNIT_ASSERT(v7 * v2 == Vector2(2.f, 4.f));
        CPPUNIT_ASSERT(v7 * v2 == v2 * v7);
    }
};

class TestVectorDivisionVector2 : public CppUnit::TestCase {
public:
    TestVectorDivisionVector2()
        : CppUnit::TestCase("vector division of vector2") {}
    void runTest() override {
        Vector2 v1 = Vector2(1.f);
        Vector2 v2 = Vector2(2.f);
        Vector2 v3 = Vector2(3.f);
        Vector2 v4 = Vector2(4.f);
        Vector2 v6 = Vector2(6.f);
        Vector2 v7 = Vector2(4.f, 2.f);
        CPPUNIT_ASSERT(v1 / v1 == v1);
        CPPUNIT_ASSERT(v3 / v1 == v3);
        CPPUNIT_ASSERT(v2 / v2 == v1);
        v4 /= v2;
        CPPUNIT_ASSERT(v4 == v2);
        CPPUNIT_ASSERT(v7 / v2 == Vector2(2.f, 1.f));
        CPPUNIT_ASSERT(v7 / v2 != v2 / v7);
    }
};

int main(int argc, char *argv[]) {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(new TestCreateVector2());
    runner.addTest(new TestAccessPointerVector2());
    runner.addTest(new TestSetVector2());
    runner.addTest(new TestEqualsVector2());
    runner.addTest(new TestAssignmentVector2());
    runner.addTest(new TestAdditionVector2());
    runner.addTest(new TestSubtractionVector2());
    runner.addTest(new TestVectorMultiplicationVector2());
    runner.addTest(new TestVectorDivisionVector2());
    runner.run();
    return EXIT_SUCCESS;
}