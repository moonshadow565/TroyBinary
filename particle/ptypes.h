#ifndef PARTICLE_PTYPES_H
#define PARTICLE_PTYPES_H
#include "../types.hpp"
#include "../inibin.h"
#include <variant>

namespace RitoParticle {
    template<typename T>
    struct TimeValue {
        float time;
        T value;
    };

    template<typename T>
    inline T EvalTimeValues(std::vector<TimeValue<T>> const& value, T base, float time) noexcept {
        if(value.empty()) {
            return base;
        }
        if(auto f = std::find_if(value.begin(), value.end(), [time](const auto& v){
            return v.time > time;
        }); f == value.end()) {
            return value.back().value;
        } else if(f == value.begin()) {
            return value.front().value;
        } else {
            auto const first_time = (f - 1)->time;
            auto const first_value = (f - 1)->value;
            auto const delta_time = f->time - first_time;
            auto const factor = (time - first_time) / delta_time;
            auto const delta_value = f->value - first_value;
            return ((delta_value * factor) + first_value) * base;
        }
    }

    struct FlatLine {
        float base;
        float delta;
    };

    struct PTable {
        std::variant<float, FlatLine, std::vector<TimeValue<float>>> value = { 1.0f };
        float eval(std::optional<float> randomv) const {
            float time = randomv.value_or(rand() / 32767.0f);
            return std::visit([time](auto&& value) -> float {
                using T = std::decay_t<decltype(value)>;
                if constexpr(std::is_same_v<T, float>) {
                    return value;
                } else if constexpr(std::is_same_v<T, FlatLine>) {
                    return value.delta * time + value.base;
                } else {
                    return EvalTimeValues(value, 1.0f,  time);
                }
            }, value);
        }
    };

    extern bool ini_get(Ini const& ini, IniHash h, PTable& val) noexcept;

    template<typename T, size_t AXES>
    struct PVar {
        T base = {};
        std::array<T, 256> ramp = {};
        std::vector<TimeValue<T>> values = {};
        std::array<std::optional<PTable>, AXES> ptables = {};

        inline void build_ramp() noexcept {
            if constexpr(!std::is_same_v<T,float>) {
                if(values.size()) {
                    int i = 0;
                    for(auto& r: ramp) {
                        r = EvalTimeValues(values, base, i * (1.0f / 256.0f));
                        ++i;
                    }
                }
            }
        }

        inline T apply_probability(T value, std::optional<float> randomv) const noexcept {
            if constexpr(std::is_same_v<T,float>) {
                if(auto const& p = ptables[0]; p) {
                    value = value * p.value().eval(randomv);
                }
            } else {
                for (size_t i = 0; i < AXES; i++) {
                    if (auto const &p = ptables[i]; p) {
                        value[i] = value[i] * p.value().eval(randomv);
                    }
                }
            }
            return value;
        }

        inline T eval_anim(float time) const noexcept {
            if(values.empty()) {
                return base;
            } else {
                if constexpr(std::is_same_v<T,float>) {
                    return EvalTimeValues(values, base, time);
                } else {
                    return ramp[static_cast<uint8_t>(time * 256.0f)];
                }
            }
        }

        inline T eval(float time, std::optional<float> randv) const noexcept {
            return apply_probability(eval_anim(time), randv);
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
