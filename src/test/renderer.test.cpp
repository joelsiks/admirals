#include <SDL_vulkan.h>
#include <VK2D/VK2D.h>
#include <VK2D/Validation.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct AssetStore {
    VK2DTexture texture;
};

void render_frame(const AssetStore assets) {
    vk2dRendererDrawTexture(assets.texture, 0, 0, 0.4, 0.4, 0, 0, 0, 0, 0, 1400,
                            1400);
}

int check_quit() {
    bool quit = false;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
    SDL_PumpEvents();
    return quit;
}

int main(int argc, char *argv[]) {
    SDL_Window *window = SDL_CreateWindow(
        "VK2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
        WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    VK2DRendererConfig config = {VK2D_MSAA_32X, VK2D_SCREEN_MODE_IMMEDIATE,
                                 VK2D_FILTER_TYPE_NEAREST};
    VK2DStartupOptions options = {true, true, true, "error.txt", false};

    if (vk2dRendererInit(window, config, &options) < 0) {
        return -1;
    }

    VK2DCameraSpec camera = {
        VK2D_CAMERA_TYPE_DEFAULT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1, 0};
    vk2dRendererSetCamera(camera);

    // Load assets
    VK2DTexture texture = vk2dTextureLoad("assets/test.jpg");
    vec4 clear = {1, 1, 1, 1};
    AssetStore assets = {texture};

    // Start render loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        quit = check_quit();
        vk2dRendererStartFrame(clear);
        render_frame(assets);
        vk2dRendererEndFrame();
    }

    // Free resources
    vk2dRendererWait();
    vk2dTextureFree(texture);
    return EXIT_SUCCESS;
}