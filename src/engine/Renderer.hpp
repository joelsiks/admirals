#pragma once

#include <memory>
#include <string>
#include <vector>

#include <VK2D/VK2D.h>

#include "DataObjects.hpp"
#include "IDrawable.hpp"

namespace admirals::renderer {

typedef std::vector<std::shared_ptr<IDrawable>> DrawableCollection;

class Renderer {
public:
    Renderer(const std::string &name, int width, int height);
    ~Renderer();

    int init(bool debug);
    void render(const DrawableCollection &drawable);

    static void drawLine(const Vector2 &p1, const Vector2 &p2, const Color &color);

    static void drawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void drawText(const VK2DTexture font, const Vector2 &position,
                         const Color &color, const std::string &text);

    static void drawTexture(const VK2DTexture texture, const Vector2 &position, const Vector2 &orgSize, const Vector2 &drawSize);

private:
    int m_windowWidth;
    int m_windowHeight;
    SDL_Window *m_window;
};

} // namespace admirals::renderer