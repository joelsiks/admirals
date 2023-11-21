#include <cmath>

#include <VK2D/Constants.h>
#include <VK2D/Renderer.h>

#include "DataObjects.hpp"
#include "Renderer.hpp"

using namespace admirals::renderer;

void RenderFont(const VK2DTexture font, const admirals::Vector2 &postion,
                const char *text) {
    float x = postion.x();
    float y = postion.y();
    float ox = x;
    for (int i = 0; i < (int)strlen(text); i++) {
        if (text[i] != '\n') {
            vk2dRendererDrawTexture(font, x, y, 2, 2, 0, 0, 0,
                                    (text[i] * 8) % 128,
                                    floorf(text[i] / 16) * 16, 8, 16);
            x += 8 * 2;
        } else {
            x = ox;
            y += 16 * 2;
        }
    }
}

Renderer::Renderer(const std::string &name, int width, int height,
                   bool debugRendering)
    : m_context({width, height, debugRendering}) {
    this->m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, width, height,
                                      SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}

Renderer::~Renderer() {
    vk2dRendererWait();
    SDL_DestroyWindow(this->m_window);
}

int Renderer::Init(bool debug) {
    VK2DRendererConfig config = {VK2D_MSAA_32X, VK2D_SCREEN_MODE_IMMEDIATE,
                                 VK2D_FILTER_TYPE_NEAREST};
    VK2DStartupOptions options = {debug, debug, debug, "error.txt", false};

    int code = vk2dRendererInit(this->m_window, config, &options);
    if (code < 0) {
        return code;
    }

    VK2DCameraSpec camera = {VK2D_CAMERA_TYPE_DEFAULT,
                             0,
                             0,
                             static_cast<float>(m_context.windowWidth),
                             static_cast<float>(m_context.windowHeight),
                             1,
                             0};

    vk2dRendererSetCamera(camera);
    return code;
}

void Renderer::Render(const DrawableCollection &drawable) {
    vk2dRendererStartFrame(Color::WHITE.Data());
    SDL_GetWindowSize(m_window, &m_context.windowWidth,
                      &m_context.windowHeight);
    for (const auto &d : drawable) {
        d->Render(m_context);
    }
    vk2dRendererEndFrame();
}

void Renderer::DrawLine(const Vector2 &p1, const Vector2 &p2,
                        const Color &color) {
    vk2dRendererSetColourMod(color.Data());
    vk2dRendererDrawLine(p1[0], p1[1], p2[0], p2[1]);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::DrawRectangle(const Vector2 &position, const Vector2 &size,
                             const Color &color) {
    vk2dRendererSetColourMod(color.Data());
    vk2dRendererDrawRectangle(position[0], position[1], size[0], size[1], 0, 0,
                              0);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::DrawRectangleOutline(const Vector2 &position,
                                    const Vector2 &size,
                                    const float outlineWidth,
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

void Renderer::DrawText(const Texture &font, const Vector2 &position,
                        const Color &color, const std::string &text) {
    vk2dRendererSetColourMod(color.Data());
    RenderFont(font.Data(), position, text.c_str());
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}
