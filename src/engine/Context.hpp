#pragma once
#include <chrono>

namespace admirals {

class Context {
public:
    Context() : m_deltaTime(0.0) {
        m_lastTime = std::chrono::high_resolution_clock::now();
    }
    Context(const Context &) = delete;
    ~Context() {}

    void UpdateDelta() {
        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::duration<double>(now - m_lastTime).count();
        m_lastTime = now;
    }
    double DeltaTime() const { return m_deltaTime; }

    int windowWidth;
    int windowHeight;

    // Indicates whether to draw outlines of elements for debugging purposes.
    bool renderDebugOutlines;

private:
    double m_deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
};

} // namespace admirals
