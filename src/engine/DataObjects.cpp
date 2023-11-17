#include "DataObjects.hpp"
#include <VK2D/Renderer.h>

using namespace admirals;

// Vector 2
Vector2::Vector2() {}

Vector2::Vector2(float v) : Vector2(v, v) {}

Vector2::Vector2(float x, float y) {
    this->m_0 = x;
    this->m_1 = y;
}
Vector2::~Vector2() {}

// Vector 3
Vector3::Vector3() {}

Vector3::Vector3(float v) : Vector3(v, v, v) {}

Vector3::Vector3(float x, float y, float z) : Vector2(x, y) { this->m_2 = z; }

Vector3::~Vector3() {}

// Vector 4
Vector4::Vector4() {}

Vector4::Vector4(float v) : Vector4(v, v, v, v) {}

Vector4::Vector4(float x, float y, float z, float w) : Vector3(x, y, z) {
    this->m_3 = w;
}

Vector4::~Vector4() {}

// Color
Color::Color() {}

Color::Color(float r, float g, float b, float a) : Vector4(r, g, b, a) {}

Color::~Color() {}

Color Color::FromHEX(const char *hex) {
    Color c = Color();
    vk2dColourHex(c.Data(), hex);
    return c;
}

Color Color::FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Color c = Color();
    vk2dColourRGBA(c.Data(), r, g, b, a);
    return c;
}

const Color Color::RED = Color(1, 0, 0, 1);
const Color Color::GREEN = Color(0, 1, 0, 1);
const Color Color::BLUE = Color(0, 0, 1, 1);
const Color Color::WHITE = Color(1, 1, 1, 1);
const Color Color::BLACK = Color(0, 0, 0, 1);

// Texture
Texture::Texture(VK2DTexture &m_texture) : m_texture(m_texture) {}

Texture::~Texture() { vk2dTextureFree(m_texture); }

unsigned int admirals::Texture::Width() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureWidth(m_texture);
}
unsigned int admirals::Texture::Height() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureHeight(m_texture);
}

Texture Texture::LoadFromPath(const std::string &path) {
    auto result = vk2dTextureLoad(path.c_str());
    return Texture(result);
}
