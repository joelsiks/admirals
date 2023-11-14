#pragma once
#include <vector>
#include <VK2D/VK2D.h>
#include "IDrawable.hpp"

namespace admirals {
namespace renderer {
    class Renderer
    {
    private:
        int windowWidth;
        int windowHeight;
        SDL_Window *window;
    public:
        Renderer(const char *name, const int width, const int height);
        ~Renderer();

        int init(bool debug);
        void render(const std::vector<IDrawable*> drawable);

        static void drawRectangle(const vec2 position, const vec2 size, const vec4 color);
    };
}}