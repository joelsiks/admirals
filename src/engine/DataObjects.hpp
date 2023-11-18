#pragma once

#include <VK2D/Texture.h>
#include <stdint.h>
#include <string>

namespace admirals {

class Vector2 {
protected:
    float m_0;
    float m_1;

public:
    Vector2();
    Vector2(float v);
    Vector2(float x, float y);
    ~Vector2();

    inline float x() const { return this->m_0; }
    inline void SetX(float value) { this->m_0 = value; }
    inline float y() const { return this->m_1; }
    inline void SetY(float value) { this->m_1 = value; }

    inline float *Data() const {
        // This is a hack that might not work on all architectures
        // It assumes that all floating point values are stored consecutively,
        // and in the right order.
        return (float *)&this->m_0;
    }

    inline void operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
    }

    inline void operator=(const Vector2 &in) {
        this->m_0 = in.m_0;
        this->m_1 = in.m_1;
    }

    inline bool operator==(const Vector2 &r) {
        return r.m_0 == m_0 && r.m_1 == m_1;
    }

    inline float operator[](int i) const { return this->Data()[i]; }

    inline float &operator[](int i) { return this->Data()[i]; }
};

class Vector3 : public Vector2 {
protected:
    float m_2;

public:
    Vector3();
    Vector3(float v);
    Vector3(float x, float y, float z);
    ~Vector3();

    inline float z() const { return this->m_2; }
    inline void setZ(float value) { this->m_2 = value; }
    inline Vector2 xy() const { return *this; }

    inline void operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
        this->m_2 = in;
    }

    inline void operator=(const Vector2 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
    }

    inline void operator=(const Vector3 &in) {
        this->m_0 = in.m_0;
        this->m_1 = in.m_1;
        this->m_2 = in.m_2;
    }

    inline bool operator==(const Vector3 &r) {
        return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2;
    }
};

class Vector4 : public Vector3 {
protected:
    float m_3;

public:
    Vector4();
    Vector4(float v);
    Vector4(float x, float y, float z, float w);
    ~Vector4();

    inline float w() const { return m_3; }
    inline void SetW(float value) { m_3 = value; }

    inline void operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
        this->m_2 = in;
        this->m_3 = in;
    }

    inline void operator=(const Vector2 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
    }

    inline void operator=(const Vector3 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
        this->m_2 = in.z();
    }

    inline void operator=(const Vector4 &in) {
        this->m_0 = in.m_0;
        this->m_1 = in.m_1;
        this->m_2 = in.m_2;
        this->m_3 = in.m_3;
    }

    inline bool operator==(const Vector4 &r) {
        return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2 && r.m_3 == m_3;
    }
};

class Color : public Vector4 {
public:
    Color();
    Color(float r, float g, float b, float a);
    ~Color();

    inline float r() const { return m_0; }
    inline void SetR(float value) { m_0 = value; }

    inline float g() const { return m_1; }
    inline void SetG(float value) { m_1 = value; }

    inline float b() const { return m_2; }
    inline void SetB(float value) { m_2 = value; }

    inline float a() const { return m_3; }
    inline void SetA(float value) { m_3 = value; }

    static Color FromHEX(const char *hex);
    static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    const static Color RED;
    const static Color GREEN;
    const static Color BLUE;
    const static Color WHITE;
    const static Color BLACK;
};

class Texture {
public:
    // Do not initialize to NULL, deconstructor will break renderer as the next
    // texture is not loaded before the next frame.
    Texture(VK2DTexture &m_texture);
    ~Texture();

    unsigned int Width() const;
    unsigned int Height() const;
    VK2DTexture Data() const { return m_texture; }

    static Texture LoadFromPath(const std::string &path);

private:
    VK2DTexture m_texture;
};

} // namespace admirals