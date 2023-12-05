#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL_mouse.h>
#include <SDL_video.h>
#include <VK2D/Texture.h>

#include "DataObjects.hpp"
#include "EngineContext.hpp"
#include "IDisplayLayer.hpp"

namespace admirals::renderer {

typedef std::vector<std::shared_ptr<IDisplayLayer>> DrawableLayers;

enum Cursor {
    Arrow = SDL_SYSTEM_CURSOR_ARROW,
    Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
    Hand = SDL_SYSTEM_CURSOR_HAND,
    IBeam = SDL_SYSTEM_CURSOR_IBEAM, // Also known as "text-cursor".
    No = SDL_SYSTEM_CURSOR_NO,
    Wait = SDL_SYSTEM_CURSOR_WAIT,
};

class Renderer {
public:
    Renderer(const std::string &name, int width, int height);
    ~Renderer();

    int Init(const EngineContext &ctx);
    static void Render(const EngineContext &context,
                       const DrawableLayers &drawables);

    Vector2 GetWindowSize() const;

    static void DrawLine(const Vector2 &p1, const Vector2 &p2,
                         const Color &color);

    static void DrawRectangle(const Rect &rect, const Color &color);

    static void DrawRectangle(const Vector2 &position, const Vector2 &size,
                              const Color &color);

    static void DrawRectangleOutline(const Rect &rect, float outlineWidth,
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

    static void SetCursor(Cursor cursor);

    static inline Vector2 TextFontSize(const std::string &text, float width,
                                       float height) {
        return Vector2(static_cast<float>(text.length()) * width, height);
    }

private:
    SDL_Window *m_window;
};

} // namespace admirals::renderer