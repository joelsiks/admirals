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

float Vector2::x() const { return this->m_0; }
void Vector2::setX(float value) { this->m_0 = value; }

float Vector2::y() const { return this->m_1; }
void Vector2::setY(float value) { this->m_1 = value; }

float *Vector2::data() const {
    // This is a hack that might not work on all architectures
    // It assumes that all floating point values are stored consecutively,
    // and in the right order.
    return (float *)&this->m_0;
}

void Vector2::operator=(float &in) {
    this->m_0 = in;
    this->m_1 = in;
}

void Vector2::operator=(const Vector2 &in) {
    this->m_0 = in.m_0;
    this->m_1 = in.m_1;
}

bool Vector2::operator==(const Vector2 &r) {
    return r.m_0 == m_0 && r.m_1 == m_1;
}

float Vector2::operator[](int i) const { return this->data()[i]; }

float &Vector2::operator[](int i) { return this->data()[i]; }

// Vector 3

Vector3::Vector3() {}

Vector3::Vector3(float v) : Vector3(v, v, v) {}

Vector3::Vector3(float x, float y, float z) : Vector2(x, y) { this->m_2 = z; }

Vector3::~Vector3() {}

float Vector3::z() const { return this->m_2; }

Vector2 Vector3::xy() const { return *this; }

void Vector3::setZ(float value) { this->m_2 = value; }

void Vector3::operator=(float &in) {
    this->m_0 = in;
    this->m_1 = in;
    this->m_2 = in;
}

void Vector3::operator=(const Vector2 &in) {
    this->m_0 = in.x();
    this->m_1 = in.y();
}

void Vector3::operator=(const Vector3 &in) {
    this->m_0 = in.m_0;
    this->m_1 = in.m_1;
    this->m_2 = in.m_2;
}

bool Vector3::operator==(const Vector3 &r) {
    return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2;
}

// Vector 4

Vector4::Vector4() {}

Vector4::Vector4(float v) : Vector4(v, v, v, v) {}

Vector4::Vector4(float x, float y, float z, float w) : Vector3(x, y, z) {
    this->m_3 = w;
}

Vector4::~Vector4() {}

float Vector4::w() const { return this->m_3; }
void Vector4::setW(float value) { this->m_3 = value; }

void Vector4::operator=(float &in) {
    this->m_0 = in;
    this->m_1 = in;
    this->m_2 = in;
    this->m_3 = in;
}

void Vector4::operator=(const Vector2 &in) {
    this->m_0 = in.x();
    this->m_1 = in.y();
}

void Vector4::operator=(const Vector3 &in) {
    this->m_0 = in.x();
    this->m_1 = in.y();
    this->m_2 = in.z();
}

void Vector4::operator=(const Vector4 &in) {
    this->m_0 = in.m_0;
    this->m_1 = in.m_1;
    this->m_2 = in.m_2;
    this->m_3 = in.m_3;
}

bool Vector4::operator==(const Vector4 &r) {
    return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2 && r.m_3 == m_3;
}

Color::Color() {}

Color::Color(float r, float g, float b, float a) : Vector4(r, g, b, a) {}

Color::~Color() {}

float Color::r() const { return this->m_0; }
void Color::setR(float value) { this->m_0 = value; }

float Color::g() const { return this->m_1; }
void Color::setG(float value) { this->m_1 = value; }

float Color::b() const { return this->m_2; }
void Color::setB(float value) { this->m_2 = value; }

float Color::a() const { return this->m_3; }
void Color::setA(float value) { this->m_3 = value; }

Color Color::fromHEX(const char *hex) {
    Color c = Color();
    vk2dColourHex(c.data(), hex);
    return c;
}
Color Color::fromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Color c = Color();
    vk2dColourRGBA(c.data(), r, g, b, a);
    return c;
}

const Color Color::RED = Color(1, 0, 0, 1);
const Color Color::GREEN = Color(0, 1, 0, 1);
const Color Color::BLUE = Color(0, 0, 1, 1);
const Color Color::WHITE = Color(1, 1, 1, 1);
const Color Color::BLACK = Color(0, 0, 0, 1);

Texture::Texture(VK2DTexture &m_texture) : m_texture(m_texture) {}

Texture::~Texture() { vk2dTextureFree(m_texture); }

unsigned int admirals::Texture::width() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureWidth(m_texture);
}
unsigned int admirals::Texture::height() const {
    if (m_texture == NULL) {
        return 0;
    }
    return vk2dTextureHeight(m_texture);
}

VK2DTexture admirals::Texture::data() const { return m_texture; }

Texture Texture::loadFromPath(const std::string &path) {
    auto result = vk2dTextureLoad(path.c_str());
    return Texture(result);
}
