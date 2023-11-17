#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL_video.h>
#include <VK2D/Texture.h>

#include "DataObjects.hpp"
#include "IDrawable.hpp"

namespace admirals::renderer {

typedef std::vector<std::shared_ptr<IDrawable>> DrawableCollection;

class Renderer {
public:
    Renderer(const std::string &name, int width, int height,
             bool debugRendering);
    ~Renderer();

    int init(bool debug);
    void render(const DrawableCollection &drawable);
    inline RendererContext context() const { return m_context; }

    static void drawLine(const Vector2 &p1, const Vector2 &p2,
                         const Color &color);

    static void drawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void drawRectangleOutline(const Vector2 &position,
                                     const Vector2 &size,
                                     const float outlineWidth,
                                     const Color &color);

    static void drawTexture(const Texture &texture, const Vector2 &position,
                            const Vector2 &scale);

    static void drawText(const Texture &font, const Vector2 &position,
                         const Color &color, const std::string &text);

    static void drawTexture(const VK2DTexture texture, const Vector2 &position,
                            const Vector2 &orgSize, const Vector2 &drawSize);

private:
    RendererContext m_context;
    SDL_Window *m_window;
};

} // namespace admirals::renderer