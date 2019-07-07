#ifndef TYPES_HPP
#define TYPES_HPP
#include <cinttypes>
#include <array>
#include <vector>
#include <variant>
#include <optional>

struct ColorF;
struct Vec4;
struct Vec3;
struct Vec2;
struct Mtx44;
struct Box3D;


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
struct ColorB {
    uint8_t r,g,b,a;
    inline constexpr static size_t size = 4;
    inline uint8_t &operator[](size_t idx) & {
        return *(reinterpret_cast<uint8_t *>(this) + idx);
    }
    inline uint8_t const &operator[](size_t idx) const & {
        return *(reinterpret_cast<uint8_t const *>(this) + idx);
    }
};
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
struct Mtx44 {
    using array_t = float[4];
    float m[4][4] = {};
    inline array_t& operator[](size_t index) & {
        return m[index];
    }
    inline array_t const& operator[](size_t index) const& {
        return m[index];
    }
};
struct Box3D {
    Vec3 min;
    Vec3 max;
};

#define ColorFInf ColorF{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec4Inf Vec4{INFINITY, INFINITY, INFINITY, INFINITY}
#define Vec3Inf Vec3{INFINITY, INFINITY, INFINITY}
#define Vec2Inf Vec2{INFINITY, INFINITY}


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

inline auto TransformNormal(Vec2 const& vec, Mtx44 const& mtx) {
    return Vec2 {
        mtx[0][0] * vec.x + mtx[1][0] * vec.y,
        mtx[0][1] * vec.x + mtx[1][1] * vec.y,
    };
}

inline auto TransformNormal(Vec3 const& vec, Mtx44 const& mtx) {
    return Vec3 {
        mtx[0][0] * vec.x + mtx[1][0] * vec.y + mtx[2][0] * vec.z,
        mtx[0][1] * vec.x + mtx[1][1] * vec.y + mtx[2][1] * vec.z,
        mtx[0][2] * vec.x + mtx[1][2] * vec.y + mtx[2][2] * vec.z,
    };
}

#endif // TYPES_HPP
