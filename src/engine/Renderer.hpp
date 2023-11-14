#pragma once

#include <memory>
#include <vector>

#include "IDrawable.hpp"
#include <VK2D/VK2D.h>

namespace admirals {
namespace renderer {
class Renderer {
private:
    int m_windowWidth;
    int m_windowHeight;
    SDL_Window *m_window;

public:
    Renderer(const char *name, int width, int height);
    ~Renderer();

    int init(bool debug);
    void render(const std::vector<std::shared_ptr<IDrawable>> &drawable);

    static void drawRectangle(const vec2 position, const vec2 size,
                              const vec4 color);
};
} // namespace renderer
} // namespace admirals