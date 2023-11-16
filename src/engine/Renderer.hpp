#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL_video.h>
#include <VK2D/Texture.h>

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
    inline RendererContext context() const { return m_context; }

    static void drawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void drawText(const VK2DTexture font, const Vector2 &position,
                         const Color &color, const std::string &text);

private:
    RendererContext m_context;
    SDL_Window *m_window;
};

} // namespace renderer
} // namespace admirals