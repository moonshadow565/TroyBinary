#ifndef PARTICLE_PTYPES_H
#define PARTICLE_PTYPES_H
#include "../types.hpp"
#include "../inibin.h"

namespace RitoParticle {
    struct PTable {
        struct FlatLine {
            float base;
            float delta;
        };
        struct ValueTime {
            float value;
            float time;
        };
        std::variant<float, FlatLine, std::vector<ValueTime>> value = { 1.0f };
    };

    extern bool ini_get(Ini const& ini, IniHash h, PTable& val) noexcept;

    template<typename T, size_t AXES>
    struct PVar {
        T base;
        std::array<T, 256> ramp;
        std::vector<float> key_times;
        std::vector<T> key_values;
        std::array<std::optional<PTable>, AXES> ptables;
        inline void build_ramp() noexcept {
            int i = 0;
            for(auto& r: ramp) {
                r = eval_anim_ex(i * (1.0f / 256.0f));
                ++i;
            }
        }
        inline T eval_anim_ex(float time) const {
            size_t idx = 0;
            size_t count = key_times.size();
            if(count == 0) {
                return base;
            }
            for(idx = 1; idx < count && key_times[idx] <= time; ++idx);
            if(idx == count) {
                if(key_times[0] > time) {
                    idx = 0;
                } else {
                    idx = count - 1;
                }
            }
        }
        inline T eval(float time = 0.0f, float randomv = 0.0f) const noexcept {
            // TODO: implement this
            return T{};
        }
    };

    using PFloat = PVar<float, 1>;
    using PVec2 = PVar<Vec2, 2>;
    using PVec3 = PVar<Vec3, 3>;
    using PVec4 = PVar<Vec4, 4>;
    using PColor = PVar<ColorF, 4>;

    extern bool ini_get(Ini const& ini, IniHash h, PFloat& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, PVec2& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, PVec3& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, PVec4& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, PColor& val) noexcept;

    template<typename T>
    struct Flex {
        T value = {};
        size_t idx = 0;
    };
    using FlexFloat = Flex<float>;
    using FlexPFloat = Flex<PFloat>;
    using FlexPVec2 = Flex<PVec2>;
    using FlexPVec3 = Flex<PVec3>;
    using FlexPVec4 = Flex<PVec4>;
    using FlexPColor = Flex<PColor>;

    extern bool ini_get(Ini const& ini, IniHash h, FlexFloat& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, FlexPFloat& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, FlexPVec2& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, FlexPVec3& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, FlexPVec4& val) noexcept;

    extern bool ini_get(Ini const& ini, IniHash h, FlexPColor& val) noexcept;

    // either render order or LOD
    enum class Importance : uint32_t {
        Low = 0,
        Normal = 1,
        High = 2,
        NotWhenHigh = 3,
    };

    // UV coordinate useage, LockAlpha unknown (makes sure alpha is always 1 maybe?)
    enum class UvMode : uint32_t {
        Default = 0 ,
        ScreenSpace = 1,
        LockAlpha = 2,
    };

    // for trail particles, no idea
    enum class TrailMode : uint32_t {
        Default = 0,
        Wake = 1,
    };

    // for beam particles, no idea what for
    enum class BeamMode : uint32_t {
        Default = 0,
        Arbitary = 1,
    };

    // how the color texture is read, do those referr to particle properties?
    enum class ColorLookupType : uint32_t {
        Constant = 0,
        Lifetime = 1,
        Velocity = 2,
        BirthRandom = 3,
    };

    // Axis the particle orbits around
    enum class FixedOrbitType : uint32_t {
        WorldX = 0,
        WorldY = 1,
        WorldZ = 2,
        WorldNegX = 3,
        WorldNegY = 4,
        WorldNegZ = 5,
    };

    // Orients the particle towards camera, or along an axis (world or local space?)
    enum class Orientation : uint32_t {
        Camera = 0,
        WorldX = 1,
        WorldY = 2,
        WorldZ = 3,
    };
}

#endif // PARTICLE_PTYPES_H
