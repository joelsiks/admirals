/// \file debug.c
/// \author Paolo Mazzon
/// \brief This is meant to be included once in each example project for the overlay
#include "debug.h"

static VK2DTexture gDebugFont = NULL;
static SDL_Window *gWindow;

void debugInit(SDL_Window *window) {
	gWindow = window;
	gDebugFont = vk2dTextureLoad("assets/font.png");
}

// Very basic and simple font renderer for the font in this test specifically
void debugRenderFont(float x, float y, const char *text) {
	float ox = x;
	for (int i = 0; i < (int)strlen(text); i++) {
		if (text[i] != '\n') {
			vk2dRendererDrawTexture(gDebugFont, x, y, 2, 2, 0, 0, 0, (text[i] * 8) % 128, floorf(text[i] / 16) * 16, 8, 16);
			x += 8 * 2;
		} else {
			x = ox;
			y += 16 * 2;
		}
	}
}

void debugRenderOverlay() {
	vk2dRendererLockCameras(VK2D_DEFAULT_CAMERA);
	char title[50];
	VK2DRendererConfig conf = vk2dRendererGetConfig();
	sprintf(title, "Vulkan2D [%0.2fms] [%0.2ffps] %ix MSAA", vk2dRendererGetAverageFrameTime(), 1000 / vk2dRendererGetAverageFrameTime(), conf.msaa);
	vk2dRendererSetColourMod(VK2D_BLACK);
	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);
	vk2dDrawRectangle(0, 0, (float)w, 17*2);
	vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
	debugRenderFont(0, 0, title);
	vk2dRendererUnlockCameras();
}

void debugCleanup() {
	vk2dTextureFree(gDebugFont);
	gDebugFont = NULL;
}