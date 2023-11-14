#include "Renderer.hpp"
#include "cmath"

namespace admirals {
namespace renderer {

static void RenderFont(const VK2DTexture font, const vec2 postion,
                       const char *text) {
    float x = postion[0];
    float y = postion[1];
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

Renderer::Renderer(const char *name, int width, int height) {
    this->m_windowWidth = width;
    this->m_windowHeight = height;
    this->m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, width, height,
                                      SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}

Renderer::~Renderer() {
    vk2dRendererWait();
    SDL_DestroyWindow(this->m_window);
}

int Renderer::init(bool debug) {
    VK2DRendererConfig config = {VK2D_MSAA_32X, VK2D_SCREEN_MODE_IMMEDIATE,
                                 VK2D_FILTER_TYPE_NEAREST};
    VK2DStartupOptions options = {debug, debug, debug, "error.txt", false};

    int code = vk2dRendererInit(this->m_window, config, &options);
    if (code < 0) {
        return code;
    }

    VK2DCameraSpec camera = {
        VK2D_CAMERA_TYPE_DEFAULT,    0, 0, (float)this->m_windowWidth,
        (float)this->m_windowHeight, 1, 0};

    vk2dRendererSetCamera(camera);
    return code;
}

void Renderer::render(const std::vector<std::shared_ptr<IDrawable>> &drawable) {
    vk2dRendererStartFrame(VK2D_WHITE);
    for (const auto &d : drawable) {
        d->render();
    }
    vk2dRendererEndFrame();
}

void Renderer::drawRectangle(const vec2 position, const vec2 size,
                             const vec4 color) {
    vk2dRendererSetColourMod(color);
    vk2dRendererDrawRectangle(position[0], position[1], size[0], size[1], 0, 0,
                              0);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

void Renderer::drawText(const VK2DTexture font, const vec2 position,
                        const vec4 color, const char *text) {
    vk2dRendererSetColourMod(color);
    RenderFont(font, position, text);
    vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
}

} // namespace renderer
} // namespace admirals