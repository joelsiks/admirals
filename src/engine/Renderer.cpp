#include <cmath>

#include <VK2D/Constants.h>
#include <VK2D/Renderer.h>

#include "DataObjects.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::renderer;

void RenderFont(VK2DTexture font, const Vector2 &postion, const Vector2 &scale,
                const char *text) {
    float x = postion.x();
    float y = postion.y();
    const float ox = x;
    for (int i = 0; i < (int)strlen(text); i++) {
        if (text[i] != '\n') {
            vk2dRendererDrawTexture(
                font, x, y, scale.x(), scale.y(), 0.f, 0.f, 0.f,
                static_cast<float>((text[i] * 8) % 128),
                floorf(static_cast<float>(text[i]) / 16.f) * 16.f, 8.f, 16.f);
            x += 8 * scale.x();
        } else {
            x = ox;
            y += 16 * scale.x();
        }
    }
}

Renderer::Renderer(const std::string &name, int width, int height) {
    this->m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, width, height,
                                      SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}

Renderer::~Renderer() {
    vk2dRendererWait();
    SDL_DestroyWindow(this->m_window);
}

int Renderer::Init(const EngineContext &ctx) {
    const VK2DRendererConfig config = {
        VK2D_MSAA_32X, VK2D_SCREEN_MODE_IMMEDIATE, VK2D_FILTER_TYPE_NEAREST};
    const bool debug =
        Engine::HasDebugMask(ctx.debug, EngineDebugMode::DebugEnabled);
    VK2DStartupOptions options = {debug, debug, debug, "error.txt", false};

    const int code = vk2dRendererInit(this->m_window, config, &options);
    if (code < 0) {
        return code;
    }

    const VK2DCameraSpec camera = {
        VK2D_CAMERA_TYPE_DEFAULT, 0, 0, ctx.windowSize.x(),
        ctx.windowSize.y(),       1, 0};

    vk2dRendererSetCamera(camera);

    return code;
}

Vector2 Renderer::GetWindowSize() const {
    int width;
    int height;
    SDL_GetWindowSize(m_window, &width, &height);
    return Vector2(static_cast<float>(width), static_cast<float>(height));
}

void DrawDebugFpsInformation(const EngineContext &ctx) {
    const std::string fpsString =
        "DT = " + std::to_string(ctx.deltaTime) +
        ", FPS: " + std::to_string(1.f / ctx.deltaTime);

    Renderer::DrawText(*ctx.fontTexture, Vector2(0), Color::RED, fpsString);
}

void Renderer::Render(const EngineContext &ctx,
                      const DrawableLayers &drawables) {
    vk2dRendererStartFrame(Color::WHITE.Data());
    for (const auto &drawable : drawables) {
        if (drawable == nullptr) {
            continue;
        }

        drawable->Render(ctx);
    }

    if (Engine::HasDebugMask(ctx.debug, EngineDebugMode::DebugEnabled |
                                            EngineDebugMode::DebugRendering)) {
        Renderer::DrawRectangleOutline(ctx.displayPort, 3, Color::BLUE);
        DrawDebugFpsInformation(ctx);
    }

    vk2dRendererEndFrame();
}

void Renderer::DrawLine(const Vector2 &p1, const Vector2 &p2,
                        const Color &color) {
    vk2dRendererSetColourMod(color.Data());
    vk2dRendererDrawLine(p1[0], p1[1], p2[0], p2[1]);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::DrawRectangle(const Rect &rect, const Color &color) {
    Renderer::DrawRectangle(rect.Position(), rect.Size(), color);
}

void Renderer::DrawRectangle(const Vector2 &position, const Vector2 &size,
                             const Color &color) {
    vk2dRendererSetColourMod(color.Data());
    vk2dRendererDrawRectangle(position[0], position[1], size[0], size[1], 0, 0,
                              0);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::DrawRectangleOutline(const Rect &rect, float outlineWidth,
                                    const Color &color) {
    Renderer::DrawRectangleOutline(rect.Position(), rect.Size(), outlineWidth,
                                   color);
}

void Renderer::DrawRectangleOutline(const Vector2 &position,
                                    const Vector2 &size, float outlineWidth,
                                    const Color &color) {
    vk2dRendererSetColourMod(color.Data());
    vk2dRendererDrawRectangleOutline(position[0], position[1], size[0], size[1],
                                     0, 0, 0, outlineWidth);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::DrawTexture(const Texture &texture, const Vector2 &position,
                           const Vector2 &scale) {
    vk2dRendererDrawTexture(texture.Data(), position.x(), position.y(),
                            scale.x(), scale.y(), 0, 0, 0, 0, 0,
                            texture.Width(), texture.Height());
}

void Renderer::DrawSprite(const Texture &texture, const Vector2 &position,
                          const Vector2 &texOffset, const Vector2 &texSize,
                          const Vector2 &scale) {
    vk2dRendererDrawTexture(texture.Data(), position.x(), position.y(),
                            scale.x(), scale.y(), 0, 0, 0, texOffset.x(),
                            texOffset.y(), texSize.x(), texSize.y());
}

void Renderer::DrawText(const Texture &font, const Vector2 &position,
                        const Color &color, const std::string &text,
                        const Vector2 &scale) {
    vk2dRendererSetColourMod(color.Data());
    RenderFont(font.Data(), position, scale, text.c_str());
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::SetCursor(Cursor cursor) {
    SDL_Cursor *c;
    c = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(cursor));
    SDL_SetCursor(c);
}