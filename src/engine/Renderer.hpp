#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL_video.h>
#include <VK2D/Texture.h>

#include "DataObjects.hpp"
#include "EngineContext.hpp"
#include "IDrawable.hpp"

namespace admirals::renderer {

typedef std::vector<std::shared_ptr<IDrawable>> DrawableCollection;

class Renderer {
public:
    Renderer(const std::string &name, int width, int height);
    ~Renderer();

    int Init(const EngineContext &context);
    static void Render(const EngineContext &context,
                       const DrawableCollection &drawables);

    Vector2 GetWindowSize() const;

    static void DrawLine(const Vector2 &p1, const Vector2 &p2,
                         const Color &color);

    static void DrawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void DrawRectangleOutline(const Vector2 &position,
                                     const Vector2 &size, float outlineWidth,
                                     const Color &color);

    static void DrawTexture(const Texture &texture, const Vector2 &position,
                            const Vector2 &scale);

    static void DrawSprite(const Texture &texture, const Vector2 &position,
                           const Vector2 &texOffset, const Vector2 &texSize,
                           const Vector2 &scale);

    static void DrawText(const Texture &font, const Vector2 &position,
                         const Color &color, const std::string &text);

private:
    SDL_Window *m_window;
};

} // namespace admirals::renderer