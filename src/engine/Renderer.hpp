#pragma once

#include <memory>
#include <string>
#include <vector>

#include <VK2D/VK2D.h>

#include "DataObjects.hpp"
#include "IDrawable.hpp"

namespace admirals {
namespace renderer {

typedef std::vector<std::shared_ptr<IDrawable>> DrawableCollection;

class Renderer {
public:
    Renderer(const std::string &name, int width, int height);
    ~Renderer();

    int init(bool debug);
    void render(const DrawableCollection &drawable);

    static void drawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void drawTexture(const Texture &texture, const Vector2 &position,
                            const Vector2 &drawSize);

    static void drawText(const Texture &font, const Vector2 &position,
                         const Color &color, const std::string &text);

private:
    int m_windowWidth;
    int m_windowHeight;
    SDL_Window *m_window;
};

} // namespace renderer
} // namespace admirals