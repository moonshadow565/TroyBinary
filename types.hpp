#ifndef TYPES_HPP
#define TYPES_HPP
#include <cinttypes>
#include <array>
#include <vector>
#include <variant>
#include <optional>

struct ColorF {
    float r,g,b,a;
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
    inline constexpr static size_t size = 4;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
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
    inline constexpr static size_t size = 3;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
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
    inline constexpr static size_t size = 2;
    inline float &operator[](size_t idx) & {
        return *(reinterpret_cast<float *>(this) + idx);
    }
    inline float const &operator[](size_t idx) const & {
        return *(reinterpret_cast<float const *>(this) + idx);
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
struct Box3D {
    Vec3 min;
    Vec3 max;
};


#define ColorFInf ColorF{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec4Inf Vec4{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec3Inf Vec3{INFINITY, INFINITY, INFINITY}
#define Vec2Inf Vec2{INFINITY, INFINITY}

#endif // TYPES_HPP
