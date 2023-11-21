#pragma once

#include <VK2D/Texture.h>
#include <cmath>
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

    inline float Magnitude() const { return std::sqrt(m_0 * m_0 + m_1 * m_1); }

    inline float Distance(const Vector2 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        return std::sqrt(a * a + b * b);
    }

    inline Vector2 &operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
        return *this;
    }

    inline Vector2 &operator=(const Vector2 &in) {
        if (this != &in) {
            this->m_0 = in.m_0;
            this->m_1 = in.m_1;
        }
        return *this;
    }

    inline bool operator==(const Vector2 &r) const {
        return r.m_0 == m_0 && r.m_1 == m_1;
    }

    inline Vector2 operator+(const Vector2 &r) const {
        return Vector2(m_0 + r.m_0, m_1 + r.m_1);
    }

    inline Vector2 &operator+=(const Vector2 &r) {
        m_0 += r.m_0;
        m_1 += r.m_1;
        return *this;
    }

    inline Vector2 operator-(const Vector2 &r) const {
        return Vector2(m_0 + r.m_0, m_1 + r.m_1);
    }

    inline Vector2 &operator-=(const Vector2 &r) {
        m_0 -= r.m_0;
        m_1 -= r.m_1;
        return *this;
    }

    inline Vector2 operator*(const Vector2 &r) const {
        return Vector2(m_0 * r.m_0, m_1 * r.m_1);
    }

    inline Vector2 operator*(float r) const {
        return Vector2(m_0 * r, m_1 * r);
    }

    inline Vector2 &operator*=(const Vector2 &r) {
        m_0 *= r.m_0;
        m_1 *= r.m_1;
        return *this;
    }

    inline Vector2 &operator*=(float r) {
        m_0 *= r;
        m_1 *= r;
        return *this;
    }

    inline Vector2 operator/(const Vector2 &r) const {
        return Vector2(m_0 / r.m_0, m_1 / r.m_1);
    }

    inline Vector2 operator/(float r) const {
        return Vector2(m_0 / r, m_1 / r);
    }

    inline Vector2 &operator/=(const Vector2 &r) {
        m_0 /= r.m_0;
        m_1 /= r.m_1;
        return *this;
    }

    inline Vector2 &operator/=(float r) {
        m_0 /= r;
        m_1 /= r;
        return *this;
    }

    inline float operator[](int i) const { return this->Data()[i]; }

    inline float &operator[](int i) { return this->Data()[i]; }

    static inline float Distance(const Vector2 &v1, const Vector2 &v2) {
        return v1.Distance(v2);
    }
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

    inline float Magnitude() const {
        return std::sqrt(m_0 * m_0 + m_1 * m_1 + m_2 * m_2);
    }

    inline float Distance(const Vector3 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        return std::sqrt(a * a + b * b + c * c);
    }

    inline Vector3 &operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
        this->m_2 = in;
        return *this;
    }

    inline Vector3 &operator=(const Vector2 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
        return *this;
    }

    inline Vector3 &operator=(const Vector3 &in) {
        if (this != &in) {
            this->m_0 = in.m_0;
            this->m_1 = in.m_1;
            this->m_2 = in.m_2;
        }
        return *this;
    }

    inline bool operator==(const Vector3 &r) const {
        return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2;
    }

    inline Vector3 operator+(const Vector3 &r) const {
        return Vector3(m_0 + r.m_0, m_1 + r.m_1, m_2 + r.m_2);
    }

    inline Vector3 &operator+=(const Vector3 &r) {
        m_0 += r.m_0;
        m_1 += r.m_1;
        m_2 += r.m_2;
        return *this;
    }

    inline Vector3 operator-(const Vector3 &r) const {
        return Vector3(m_0 + r.m_0, m_1 + r.m_1, m_2 + r.m_2);
    }

    inline Vector3 &operator-=(const Vector3 &r) {
        m_0 -= r.m_0;
        m_1 -= r.m_1;
        m_2 -= r.m_2;
        return *this;
    }

    inline Vector3 operator*(const Vector3 &r) const {
        return Vector3(m_0 * r.m_0, m_1 * r.m_1, m_2 * r.m_2);
    }

    inline Vector3 operator*(float r) const {
        return Vector3(m_0 * r, m_1 * r, m_2 * r);
    }

    inline Vector3 &operator*=(const Vector3 &r) {
        m_0 *= r.m_0;
        m_1 *= r.m_1;
        m_2 *= r.m_2;
        return *this;
    }

    inline Vector3 &operator*=(float r) {
        m_0 *= r;
        m_1 *= r;
        m_2 *= r;
        return *this;
    }

    inline Vector3 operator/(const Vector3 &r) const {
        return Vector3(m_0 / r.m_0, m_1 / r.m_1, m_2 / r.m_2);
    }

    inline Vector3 operator/(float r) const {
        return Vector3(m_0 / r, m_1 / r, m_2 / r);
    }

    inline Vector3 &operator/=(const Vector3 &r) {
        m_0 /= r.m_0;
        m_1 /= r.m_1;
        m_2 /= r.m_2;
        return *this;
    }

    inline Vector3 &operator/=(float r) {
        m_0 /= r;
        m_1 /= r;
        m_2 /= r;
        return *this;
    }

    static inline float Distance(const Vector3 &v1, const Vector3 &v2) {
        return v1.Distance(v2);
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

    inline float Magnitude() const {
        return std::sqrt(m_0 * m_0 + m_1 * m_1 + m_2 * m_2 + m_3 * m_3);
    }

    inline float Distance(const Vector4 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        const float d = m_3 - v.m_3;
        return std::sqrt(a * a + b * b + c * c + d * d);
    }

    inline Vector4 &operator=(float &in) {
        this->m_0 = in;
        this->m_1 = in;
        this->m_2 = in;
        this->m_3 = in;
        return *this;
    }

    inline Vector4 &operator=(const Vector2 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
        return *this;
    }

    inline Vector4 &operator=(const Vector3 &in) {
        this->m_0 = in.x();
        this->m_1 = in.y();
        this->m_2 = in.z();
        return *this;
    }

    inline Vector4 &operator=(const Vector4 &in) {
        if (this != &in) {
            this->m_0 = in.m_0;
            this->m_1 = in.m_1;
            this->m_2 = in.m_2;
            this->m_3 = in.m_3;
        }
        return *this;
    }

    inline bool operator==(const Vector4 &r) const {
        return r.m_0 == m_0 && r.m_1 == m_1 && r.m_2 == m_2 && r.m_3 == m_3;
    }

    inline Vector4 operator+(const Vector4 &r) const {
        return Vector4(m_0 + r.m_0, m_1 + r.m_1, m_2 + r.m_2, m_3 + r.m_3);
    }

    inline Vector4 &operator+=(const Vector4 &r) {
        m_0 += r.m_0;
        m_1 += r.m_1;
        m_2 += r.m_2;
        m_3 += r.m_3;
        return *this;
    }

    inline Vector4 operator-(const Vector4 &r) const {
        return Vector4(m_0 + r.m_0, m_1 + r.m_1, m_2 + r.m_2, m_3 + r.m_3);
    }

    inline Vector4 &operator-=(const Vector4 &r) {
        m_0 -= r.m_0;
        m_1 -= r.m_1;
        m_2 -= r.m_2;
        m_3 -= r.m_3;
        return *this;
    }

    inline Vector4 operator*(const Vector4 &r) const {
        return Vector4(m_0 * r.m_0, m_1 * r.m_1, m_2 * r.m_2, m_3 * r.m_3);
    }

    inline Vector4 operator*(float r) const {
        return Vector4(m_0 * r, m_1 * r, m_2 * r, m_3 * r);
    }

    inline Vector4 &operator*=(const Vector4 &r) {
        m_0 *= r.m_0;
        m_1 *= r.m_1;
        m_2 *= r.m_2;
        m_3 *= r.m_3;
        return *this;
    }

    inline Vector4 &operator*=(float r) {
        m_0 *= r;
        m_1 *= r;
        m_2 *= r;
        m_3 *= r;
        return *this;
    }

    inline Vector4 operator/(const Vector4 &r) const {
        return Vector4(m_0 / r.m_0, m_1 / r.m_1, m_2 / r.m_2, m_3 / r.m_3);
    }

    inline Vector4 operator/(float r) const {
        return Vector4(m_0 / r, m_1 / r, m_2 / r, m_3 / r);
    }

    inline Vector4 &operator/=(const Vector4 &r) {
        m_0 /= r.m_0;
        m_1 /= r.m_1;
        m_2 /= r.m_2;
        m_3 /= r.m_3;
        return *this;
    }

    inline Vector4 &operator/=(float r) {
        m_0 /= r;
        m_1 /= r;
        m_2 /= r;
        m_3 /= r;
        return *this;
    }

    static inline float Distance(const Vector4 &v1, const Vector4 &v2) {
        return v1.Distance(v2);
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

    // should be uint32
    float Width() const;
    float Height() const;
    VK2DTexture Data() const { return m_texture; }

    static Texture LoadFromPath(const std::string &path);

private:
    VK2DTexture m_texture;
};

} // namespace admirals