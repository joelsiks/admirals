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
    Renderer(const std::string &name, int width, int height,
             bool debugRendering);
    ~Renderer();

    int Init(bool debug);
    void Render(const DrawableCollection &drawables);

    inline void ToggleDebugRendering() {
        m_context.renderDebugOutlines = !m_context.renderDebugOutlines;
    }

    inline RendererContext context() const { return m_context; }

    static void DrawLine(const Vector2 &p1, const Vector2 &p2,
                         const Color &color);

    static void DrawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void DrawRectangleOutline(const Vector2 &position,
                                     const Vector2 &size,
                                     const float outlineWidth,
                                     const Color &color);

    static void DrawTexture(const Texture &texture, const Vector2 &position,
                            const Vector2 &scale);

    static void DrawText(const Texture &font, const Vector2 &position,
                         const Color &color, const std::string &text);

private:
    RendererContext m_context;
    SDL_Window *m_window;
};

} // namespace renderer
} // namespace admirals