#pragma once

#include <VK2D/Texture.h>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#undef TRANSPARENT

namespace admirals {

class Vector2 {
protected:
    float m_0 = 0;
    float m_1 = 0;

public:
    constexpr Vector2() noexcept {}
    constexpr Vector2(float v) noexcept : m_0(v), m_1(v) {}
    constexpr Vector2(float x, float y) noexcept : m_0(x), m_1(y) {}
    constexpr virtual ~Vector2() noexcept {}

    constexpr float x() const { return this->m_0; }
    inline void SetX(float value) { this->m_0 = value; }
    constexpr float y() const { return this->m_1; }
    inline void SetY(float value) { this->m_1 = value; }

    inline float *Data() const {
        // This is a hack that might not work on all architectures
        // It assumes that all floating point values are stored consecutively,
        // and in the right order.
        return (float *)&this->m_0;
    }

    inline float Magnitude() const { return std::sqrt(m_0 * m_0 + m_1 * m_1); }

    /// @brief The Euclidean distance between two points.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float Distance(const Vector2 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        return std::sqrt(a * a + b * b);
    }

    /// @brief A variant of the Manhattan distance between two points, giving
    /// the maximum distance between each component in the vectors.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float MaxDistance(const Vector2 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        return std::max(std::abs(a), std::abs(b));
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
        return Vector2(m_0 - r.m_0, m_1 - r.m_1);
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

    // NOLINTNEXTLINE
    inline float &operator[](int i) { return this->Data()[i]; }

    static inline float Distance(const Vector2 &v1, const Vector2 &v2) {
        return v1.Distance(v2);
    }

    inline std::string ToString(unsigned int decimals = 4) const {
        std::ostringstream out;
        out.precision(decimals);
        out << std::fixed << "(" << x() << ", " << y() << ")";
        return std::move(out).str();
    }
};

class Vector3 : public Vector2 {
protected:
    float m_2 = 0;

public:
    constexpr Vector3() noexcept {}
    constexpr Vector3(float v) noexcept : Vector2(v), m_2(v) {}
    constexpr Vector3(float x, float y, float z) noexcept
        : Vector2(x, y), m_2(z) {}
    constexpr virtual ~Vector3() noexcept {}

    constexpr float z() const { return this->m_2; }
    inline void SetZ(float value) { this->m_2 = value; }
    inline Vector2 xy() const { return *this; }

    inline float Magnitude() const {
        return std::sqrt(m_0 * m_0 + m_1 * m_1 + m_2 * m_2);
    }

    /// @brief The Euclidean distance between two points.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float Distance(const Vector3 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        return std::sqrt(a * a + b * b + c * c);
    }

    /// @brief A variant of the Manhattan distance between two points, giving
    /// the maximum distance between each component in the vectors.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float MaxDistance(const Vector3 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        return std::max(std::max(std::abs(a), std::abs(b)), std::abs(c));
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
        return Vector3(m_0 - r.m_0, m_1 - r.m_1, m_2 - r.m_2);
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

    inline std::string ToString(unsigned int decimals = 4) const {
        std::ostringstream out;
        out.precision(decimals);
        out << std::fixed << "(" << x() << ", " << y() << ", " << z() << ")";
        return std::move(out).str();
    }
};

class Vector4 : public Vector3 {
protected:
    float m_3 = 0;

public:
    constexpr Vector4() noexcept {}
    constexpr Vector4(float v) noexcept : Vector3(v), m_3(v) {}
    constexpr Vector4(float x, float y, float z, float w) noexcept
        : Vector3(x, y, z), m_3(w) {}
    constexpr virtual ~Vector4() noexcept {}

    constexpr float w() const { return m_3; }
    inline void SetW(float value) { m_3 = value; }

    inline float Magnitude() const {
        return std::sqrt(m_0 * m_0 + m_1 * m_1 + m_2 * m_2 + m_3 * m_3);
    }

    /// @brief The Euclidean distance between two points.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float Distance(const Vector4 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        const float d = m_3 - v.m_3;
        return std::sqrt(a * a + b * b + c * c + d * d);
    }

    /// @brief A variant of the Manhattan distance between two points, giving
    /// the maximum distance between each component in the vectors.
    /// @param v The point to measure the distance to.
    /// @return The distance between the two points.
    inline float MaxDistance(const Vector4 &v) const {
        const float a = m_0 - v.m_0;
        const float b = m_1 - v.m_1;
        const float c = m_2 - v.m_2;
        const float d = m_3 - v.m_3;
        return std::max(std::max(std::abs(a), std::abs(b)),
                        std::max(std::abs(c), std::abs(d)));
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
        return Vector4(m_0 - r.m_0, m_1 - r.m_1, m_2 - r.m_2, m_3 - r.m_3);
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

    inline std::string ToString(unsigned int decimals = 4) const {
        std::ostringstream out;
        out.precision(decimals);
        out << std::fixed << "(" << x() << ", " << y() << ", " << z() << ", "
            << w() << ")";
        return std::move(out).str();
    }
};

class Color : public Vector4 {
public:
    constexpr Color() noexcept {}
    constexpr Color(const Vector4 &v) noexcept : Vector4(v) {}
    constexpr Color(float r, float g, float b, float a) noexcept
        : Vector4(r, g, b, a) {}
    constexpr virtual ~Color() noexcept {};

    inline float r() const { return m_0; }
    inline void SetR(float value) { m_0 = value; }

    inline float g() const { return m_1; }
    inline void SetG(float value) { m_1 = value; }

    inline float b() const { return m_2; }
    inline void SetB(float value) { m_2 = value; }

    inline float a() const { return m_3; }
    inline void SetA(float value) { m_3 = value; }

    inline Color operator*(const Color &r) const {
        return Color(m_0 * r.m_0, m_1 * r.m_1, m_2 * r.m_2, m_3 * r.m_3);
    }

    inline Color &operator*=(const Color &r) {
        m_0 *= r.m_0;
        m_1 *= r.m_1;
        m_2 *= r.m_2;
        m_3 *= r.m_3;
        return *this;
    }

    static Color FromHEX(const char *hex);
    static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    inline static Color Lerp(const Color &c1, const Color &c2, float t) {
        return c1 * Vector4(1 - t) + c2 * Vector4(t);
    };

    const static Color RED;
    const static Color GREEN;
    const static Color BLUE;
    const static Color WHITE;
    const static Color BLACK;
    const static Color TRANSPARENT;
    const static Color GREY;
    const static Color LIGHT_GREY;
};

// Define colors
constexpr Color Color::RED = Color(1, 0, 0, 1);
constexpr Color Color::GREEN = Color(0, 1, 0, 1);
constexpr Color Color::BLUE = Color(0, 0, 1, 1);
constexpr Color Color::WHITE = Color(1, 1, 1, 1);
constexpr Color Color::BLACK = Color(0, 0, 0, 1);
constexpr Color Color::TRANSPARENT = Color(0, 0, 0, 0);

class Rect {
public:
    /// @brief Creates a `Rect` object representing an area in 2d space
    constexpr Rect() noexcept {}

    /// @brief Creates a `Rect` object representing an area in 2d space
    /// @param position The position of the top-left corner in the `Rect`
    /// @param size The size of the `Rect`
    constexpr Rect(const Vector2 &position, const Vector2 &size) noexcept
        : m_x(position.x()), m_y(position.y()), m_w(size.x()), m_h(size.y()) {}

    /// @brief Creates a `Rect` object representing an area in 2d space
    /// @param x The x-coordinate of the top-left corner in the `Rect`
    /// @param y The y-coordinate of the top-left corner in the `Rect`
    /// @param w The width of the `Rect`
    /// @param h The height of the `Rect`
    constexpr Rect(float x, float y, float w, float h) noexcept
        : m_x(x), m_y(y), m_w(w), m_h(h) {}

    constexpr virtual ~Rect() noexcept {}

    /// @brief Checks if a given point is located inside the bounds of the
    /// `Rect`.
    /// @param point The point to check
    /// @return `true` if the given point is located inside the bounds of the
    /// `Rect`, otherwise `false`.
    inline bool Contains(const Vector2 &point) const {
        return point.x() >= m_x && point.x() <= m_x + m_w && point.y() >= m_y &&
               point.y() <= m_y + m_h;
    };

    /// @brief Checks if a given area is fully contained inside the bounds of
    /// the `Rect`.
    /// @param rect The area to check
    /// @return `true` if the given area is fully contained inside the bounds of
    /// the `Rect`, otherwise `false`.
    inline bool Contains(const Rect &rect) const {
        return m_x <= rect.m_x && m_x + m_w >= rect.m_x + rect.m_w &&
               m_y <= rect.m_y && m_y + m_h >= rect.m_y + rect.m_h;
    };

    /// @brief Checks if a given area has some overlap with the bounds of the
    /// `Rect`.
    /// @param rect The area to check
    /// @return `true` if the given area has some overlap with the bounds of the
    /// `Rect`, otherwise `false`.
    inline bool Overlaps(const Rect &rect) const {
        return m_x < rect.m_x + rect.m_w && m_x + m_w > rect.m_x &&
               m_y < rect.m_y + rect.m_h && m_y + m_h > rect.m_y;
    };

    inline Vector2 Center() const {
        return Vector2(m_x + m_w / 2.f, m_y + m_h / 2.f);
    };

    inline float PositionX() const { return m_x; };
    inline void SetPositionX(const float x) { m_x = x; }
    inline float PositionY() const { return m_y; };
    inline void SetPositionY(const float y) { m_y = y; }

    /// @brief The top left 2d coordinate of the `Rect`
    inline Vector2 Position() const { return Vector2(m_x, m_y); };

    /// @brief The top left 2d coordinate of the `Rect`
    inline void SetPosition(const Vector2 &position) {
        m_x = position.x();
        m_y = position.y();
    }

    inline float Width() const { return m_w; };
    inline void SetWidth(const float width) { m_w = width; }
    inline float Height() const { return m_h; };
    inline void SetHeight(const float height) { m_h = height; }

    inline Vector2 Size() const { return Vector2(m_w, m_h); };
    inline void SetSize(const Vector2 &size) {
        m_w = size.x();
        m_h = size.y();
    }

    constexpr bool IsZero() const noexcept {
        return m_x == 0 && m_y == 0 && m_w == 0 && m_h == 0;
    }

    inline std::string ToString(unsigned int decimals = 4) const {
        std::ostringstream out;
        out.precision(decimals);
        out << std::fixed << "(" << m_x << ", " << m_y << " | " << m_w << " x "
            << m_h << ")";
        return std::move(out).str();
    }

private:
    float m_x = 0;
    float m_y = 0;
    float m_w = 0;
    float m_h = 0;
};

class Texture {
public:
    // Do not initialize to NULL, deconstructor will break renderer as the next
    // texture is not loaded before the next frame.
    Texture(VK2DTexture m_texture) noexcept;
    virtual ~Texture() noexcept;

    // should be uint32
    float Width() const;
    float Height() const;
    inline Vector2 Size() const { return Vector2(Width(), Height()); };
    VK2DTexture Data() const { return m_texture; }

    static Texture LoadFromPath(const std::string &path);

private:
    VK2DTexture m_texture;
};

} // namespace admirals