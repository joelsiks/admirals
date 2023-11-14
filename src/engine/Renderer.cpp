#include "Renderer.hpp"

namespace admirals {
namespace renderer {

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

} // namespace renderer
} // namespace admirals