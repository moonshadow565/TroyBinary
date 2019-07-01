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
};

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
};

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
};

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
};

struct Mtx44 {
    float m[4][4] = {};
};

#define ColorFInf ColorF{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec4Inf Vec4{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec3Inf Vec3{INFINITY, INFINITY, INFINITY}
#define Vec2Inf Vec2{INFINITY, INFINITY}

#endif // TYPES_HPP
