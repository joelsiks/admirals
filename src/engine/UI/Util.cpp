
#include <math.h>

#include <VK2D/VK2D.h>

#include "Util.hpp"

void admirals::UI::RenderFont(const VK2DTexture &font, float x, float y,
                              const char *text) {
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