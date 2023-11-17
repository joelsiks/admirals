#pragma once
#include <list>
#include <string>

namespace admirals::test {

class TestCaseBase {
public:
    TestCaseBase(const std::string &name) : m_name(name){};
    virtual void test() = 0;
    std::string name() const { return m_name; }

protected:
    void assert(bool condition, const std::string &msg) {
        if (!condition) {
            printf("\rTESTING: %s - Error: %s  \n", m_name.c_str(),
                   msg.c_str());
            throw(100);
        }
    }

    const std::string m_name;
};

class TestHandler {
public:
    TestHandler() {}
    ~TestHandler() {
        for (const auto &test : m_tests) {
            free(test);
        }
    }

    void run() {
        for (const auto &test : m_tests) {
            try {
                printf("TESTING: %s - Starting", test->name().c_str());
                test->test();
                printf("\rTESTING: %s - Passed  \n", test->name().c_str());
            } catch (const int e) {
                if (e == 100) {
                    m_failures++;
                } else {
                    throw(e);
                }
            }
        }
        if (m_failures > 0) {
            printf("TESTING: %d Failures...\n", m_failures);
        } else {
            printf("TESTING: Success!\n");
        }
    }

    void add(TestCaseBase *test) { m_tests.push_back(test); }

private:
    std::list<TestCaseBase *> m_tests;
    int m_failures = 0;
};

} // namespace admirals::test