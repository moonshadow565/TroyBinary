#ifndef TYPES_HPP
#define TYPES_HPP
#include <cinttypes>
#include <array>
#include <vector>
#include <variant>
#include <optional>

struct ColorF {
    float r,g,b,a;
    constexpr inline ColorF() noexcept
        : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
    {}
    constexpr inline ColorF(float r, float g, float b, float a) noexcept
        : r(r), g(g), b(b), a(a)
    {}
    constexpr inline ColorF(std::array<float, 4> const& other) noexcept
        : r(other[0]),
          g(other[1]),
          b(other[2]),
          a(other[3])
    {}

    inline constexpr static size_t size = 4;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
    }
};
inline auto operator * (ColorF const& l, float r) {
    return ColorF {l.r * r, l.g * r, l.b * r, l.a * r};
}
inline auto operator / (ColorF const& l, float r) {
    return ColorF {l.r / r, l.g / r, l.b / r, l.a / r};
}
inline auto operator + (ColorF const& l, float r)  {
    return ColorF {l.r + r, l.g + r, l.b + r, l.a + r};
}
inline auto operator - (ColorF const& l, float r) {
    return ColorF {l.r - r, l.g - r, l.b - r, l.a - r};
}
inline auto operator * (ColorF const& l, ColorF const&  r) {
    return ColorF {l.r * r.r, l.g * r.g, l.b * r.b, l.a * r.a};
}
inline auto operator / (ColorF const& l, ColorF const&  r) {
    return ColorF {l.r / r.r, l.g / r.g, l.b / r.b, l.a / r.a};
}
inline auto operator + (ColorF const& l, ColorF const& r) {
    return ColorF {l.r + r.r, l.g + r.g, l.b + r.b, l.a + r.a};
}
inline auto operator - (ColorF const& l, ColorF const& r) {
    return ColorF {l.r - r.r, l.g - r.g, l.b - r.b, l.a - r.a};
}


struct Vec4 {
    float x,y,z,w;
    constexpr inline Vec4() noexcept
        : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    {}
    constexpr inline Vec4(float x, float y, float z, float w) noexcept
        : x(x), y(y), z(z), w(w)
    {}
    constexpr inline Vec4(std::array<float, 4> const& other) noexcept
        : x(other[0]),
          y(other[1]),
          z(other[2]),
          w(other[3])
    {}
    constexpr inline void operator=(std::array<float, 4> const& other) noexcept {
        x = other[0];
        y = other[1];
        z = other[2];
        w = other[3];
    }

    inline constexpr static size_t size = 4;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
    }
    inline Vec4 const operator*=(float in) const {
        return Vec4(in * x, in * y, in * z, in * w);
    }

};
inline auto operator * (Vec4 const& l, float r) {
    return Vec4 {l.x * r, l.y * r, l.z * r, l.w * r};
}
inline auto operator / (Vec4 const& l, float r) {
    return Vec4 {l.x / r, l.y / r, l.z / r, l.w / r};
}
inline auto operator - (Vec4 const& l, float r) {
    return Vec4 {l.x - r, l.y - r, l.z - r, l.w - r};
}
inline auto operator + (Vec4 const& l, float r) {
    return Vec4 {l.x + r, l.y + r, l.z + r, l.w + r};
}
inline auto operator * (Vec4 const& l, Vec4 const&  r) {
    return Vec4 {l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w};
}
inline auto operator / (Vec4 const& l, Vec4 const&  r) {
    return Vec4 {l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w};
}
inline auto operator + (Vec4 const& l, Vec4 const&  r) {
    return Vec4 {l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w};
}
inline auto operator - (Vec4 const& l, Vec4 const&  r) {
    return Vec4 {l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w};
}


struct Vec3 {
    float x,y,z;

    constexpr inline Vec3() noexcept
        : x(0.0f), y(0.0f), z(0.0f)
    {}
    constexpr inline Vec3(float x, float y, float z) noexcept
        : x(x), y(y), z(z)
    {}
    constexpr inline Vec3(std::array<float, 3> const& other) noexcept
        : x(other[0]),
          y(other[1]),
          z(other[2])
    {}
    constexpr inline void operator=(std::array<float, 3> const& other) noexcept {
        x = other[0];
        y = other[1];
        z = other[2];
    }

    inline constexpr static size_t size = 3;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
    }
    inline Vec3 const operator*=(float in) const {
        return Vec3(in * x, in * y, in * z);
    }

};
inline auto operator * (Vec3 const& l, float r) {
    return Vec3 {l.x * r, l.y * r, l.z * r};
}
inline auto operator / (Vec3 const& l, float r) {
    return Vec3 {l.x / r, l.y / r, l.z / r};
}
inline auto operator - (Vec3 const& l, float r) {
    return Vec3 {l.x - r, l.y - r, l.z - r};
}
inline auto operator + (Vec3 const& l, float r) {
    return Vec3 {l.x + r, l.y + r, l.z + r};
}
inline auto operator * (Vec3 const& l, Vec3 const&  r) {
    return Vec3 {l.x * r.x, l.y * r.y, l.z * r.z};
}
inline auto operator / (Vec3 const& l, Vec3 const&  r) {
    return Vec3 {l.x / r.x, l.y / r.y, l.z / r.z};
}
inline auto operator + (Vec3 const& l, Vec3 const&  r) {
    return Vec3 {l.x + r.x, l.y + r.y, l.z + r.z};
}
inline auto operator - (Vec3 const& l, Vec3 const&  r) {
    return Vec3 {l.x - r.x, l.y - r.y, l.z - r.z};
}


struct Vec2 {
    float x,y;
    constexpr inline Vec2() noexcept
        : x(0.0f), y(0.0f)
    {}
    constexpr inline Vec2(float x, float y) noexcept
        : x(x), y(y)
    {}
    constexpr inline Vec2(std::array<float, 2> const& other) noexcept
        : x(other[0]),
          y(other[1])
    {}
    constexpr inline void operator=(std::array<float, 2> const& other) noexcept {
        x = other[0];
        y = other[1];
    }

    inline constexpr static size_t size = 2;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
    }
    inline Vec2 const operator*=(float in) const {
        return Vec2(in * x, in * y);
    }
};
inline auto operator * (Vec2 const& l, float r) {
    return Vec2 {l.x * r, l.y * r};
}
inline auto operator / (Vec2 const& l, float r) {
    return Vec2 {l.x / r, l.y / r};
}
inline auto operator - (Vec2 const& l, float r) {
    return Vec2 {l.x - r, l.y - r};
}
inline auto operator + (Vec2 const& l, float r) {
    return Vec2 {l.x + r, l.y + r};
}
inline auto operator * (Vec2 const& l, Vec2 const&  r) {
    return Vec2 {l.x * r.x, l.y * r.y};
}
inline auto operator / (Vec2 const& l, Vec2 const&  r) {
    return Vec2 {l.x / r.x, l.y / r.y};
}
inline auto operator + (Vec2 const& l, Vec2 const&  r) {
    return Vec2 {l.x + r.x, l.y + r.y};
}
inline auto operator - (Vec2 const& l, Vec2 const&  r) {
    return Vec2 {l.x - r.x, l.y - r.y};
}


struct Mtx44 {
    float m[4][4] = {};
};

#define ColorFInf ColorF{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec4Inf Vec4{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec3Inf Vec3{INFINITY, INFINITY, INFINITY}
#define Vec2Inf Vec2{INFINITY, INFINITY}

#endif // TYPES_HPP
