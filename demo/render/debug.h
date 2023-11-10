
#pragma once

#include <VK2D/VK2D.h>

void debugInit(SDL_Window *window);
void debugRenderFont(float x, float y, const char *text);
void debugRenderOverlay();
void debugCleanup();

const VK2DVertexColour SAMPLE_TRIANGLE[] = {
		{{0.0, 0.0, +0.0}, {1.0, 1.0, 0.5, 1}},
		{{400, 300, +0.0}, {1.0, 0.5, 1.0, 1}},
		{{400, 0.0, +0.0}, {0.5, 1.0, 1.0, 1}},
		{{0.0, 0.0, +0.0}, {0.5, 0.5, 1.0, 1}},
		{{0.0, 300, +0.0}, {0.0, 1.0, 0.5, 1}},
		{{400, 300, +0.0}, {1.0, 1.0, 1.0, 1}}
};
const uint32_t VERTICES = 6;
