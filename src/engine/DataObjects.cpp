#include "DataObjects.hpp"
#include <VK2D/Renderer.h>

using namespace admirals;

// Vector 2
Vector2::Vector2() noexcept {}

Vector2::Vector2(float v) noexcept : Vector2(v, v) {}

Vector2::Vector2(float x, float y) noexcept {
    this->m_0 = x;
    this->m_1 = y;
}
Vector2::~Vector2() noexcept {}

// Vector 3
Vector3::Vector3() noexcept {}

Vector3::Vector3(float v) noexcept : Vector3(v, v, v) {}

Vector3::Vector3(float x, float y, float z) noexcept : Vector2(x, y) {
    this->m_2 = z;
}

Vector3::~Vector3() noexcept {}

// Vector 4
Vector4::Vector4() noexcept {}

Vector4::Vector4(float v) noexcept : Vector4(v, v, v, v) {}

Vector4::Vector4(float x, float y, float z, float w) noexcept
    : Vector3(x, y, z) {
    this->m_3 = w;
}

Vector4::~Vector4() noexcept {}

// Rect

Rect::Rect(const Vector2 &position, const Vector2 &size) noexcept
    : Rect(position.x(), position.y(), size.x(), size.y()) {}

Rect::Rect(float x, float y, float w, float h) noexcept
    : m_x(x), m_y(y), m_w(w), m_h(h) {}

Rect::Rect() noexcept : Rect(0, 0, 0, 0) {}
Rect::~Rect() noexcept {}

// Color
Color::Color() noexcept {}

Color::Color(float r, float g, float b, float a) noexcept
    : Vector4(r, g, b, a) {}

Color::Color(const Vector4 &vec) noexcept : Vector4(vec) {}

Color::~Color() noexcept {}

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

const Color Color::RED = Color(1, 0, 0, 1);
const Color Color::GREEN = Color(0, 1, 0, 1);
const Color Color::BLUE = Color(0, 0, 1, 1);
const Color Color::WHITE = Color(1, 1, 1, 1);
const Color Color::BLACK = Color(0, 0, 0, 1);
const Color Color::TRANSPARENT = Color(0, 0, 0, 0);
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
