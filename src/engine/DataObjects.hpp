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

    float x() const;
    void setX(float value);
    float y() const;
    void setY(float value);

    float *data() const;

    void operator=(float &in);
    void operator=(const Vector2 &in);
    float operator[](int i) const;
    float &operator[](int i);
};

class Vector3 : public Vector2 {
protected:
    float m_2;

public:
    Vector3();
    Vector3(float v);
    Vector3(float x, float y, float z);
    ~Vector3();

    float z() const;
    void setZ(float value);

    void operator=(float &in);
    void operator=(const Vector3 &in);
};

class Vector4 : public Vector3 {
protected:
    float m_3;

public:
    Vector4();
    Vector4(float v);
    Vector4(float x, float y, float z, float w);
    ~Vector4();

    float w() const;
    void setW(float value);

    void operator=(float &in);
    void operator=(const Vector4 &in);
};

class Color : public Vector4 {
public:
    Color();
    Color(float r, float g, float b, float a);
    ~Color();

    float r() const;
    void setR(float value);
    float g() const;
    void setG(float value);
    float b() const;
    void setB(float value);
    float a() const;
    void setA(float value);

    static Color fromHEX(const char *hex);
    static Color fromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

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

    unsigned int width() const;
    unsigned int height() const;
    VK2DTexture data() const;

    static Texture loadFromPath(const std::string &path);

private:
    VK2DTexture m_texture;
};

} // namespace admirals