#include "DataObjects.hpp"
#include <VK2D/Renderer.h>

using namespace admirals;

Color Color::FromHEX(const char *hex) {
    const Color c = Color();
    vk2dColourHex(c.Data(), hex);
    return c;
}

Color Color::FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    const Color c = Color();
    vk2dColourRGBA(c.Data(), r, g, b, a);
    return c;
}

const Color Color::GREY = Color::FromRGBA(80, 80, 80, 255);
const Color Color::LIGHT_GREY = Color::FromRGBA(180, 180, 180, 255);

// Texture
Texture::Texture(VK2DTexture m_texture) noexcept : m_texture(m_texture) {}

Texture::~Texture() { vk2dTextureFree(m_texture); }

float admirals::Texture::Width() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureWidth(m_texture);
}
float admirals::Texture::Height() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureHeight(m_texture);
}

Texture Texture::LoadFromPath(const std::string &path) {
    VK2DTexture result = vk2dTextureLoad(path.c_str());
    return Texture(result);
}
