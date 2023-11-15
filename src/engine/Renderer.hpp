#pragma once

#include <memory>
#include <string>
#include <vector>

#include <VK2D/VK2D.h>

#include "IDrawable.hpp"

namespace admirals {
namespace renderer {

class Renderer {
public:
    Renderer(const std::string &name, int width, int height);
    ~Renderer();

    int init(bool debug);
    void render(const std::vector<std::shared_ptr<IDrawable>> &drawable);

    static void drawRectangle(const vec2 position, const vec2 size,
                              const vec4 color);

    static void drawText(const VK2DTexture font, const vec2 position,
                         const vec4 color, const char *text);

private:
    int m_windowWidth;
    int m_windowHeight;
    SDL_Window *m_window;
};

} // namespace renderer
} // namespace admirals