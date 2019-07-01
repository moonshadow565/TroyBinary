#ifndef PARTICLE_FIELDS_H
#define PARTICLE_FIELDS_H
#include "ptypes.h"
#include "../inibin.h"

namespace RitoParticle {
    struct FieldAcceleration {
        std::string name;
        bool isLocalSpace;
        PVec3 acceleration;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FieldAcceleration& val) noexcept;

    struct FieldAttraction {
        std::string name;
        PVec3 position;
        PFloat radius;
        PFloat acceleration;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FieldAttraction& val) noexcept;

    struct FieldDrag {
        std::string name;
        PVec3 position;
        PFloat radius;
        PFloat strength;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FieldDrag& val) noexcept;

    struct FieldNoise {
        std::string name;
        PVec3 position;
        PFloat radius;
        PFloat period;
        PFloat velocityDelta;
        Vec3 axisFraction;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FieldNoise& val) noexcept;

    struct FieldOrbital {
        std::string name;
        bool isLocalSpace;
        PVec3 direction;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FieldOrbital& val) noexcept;

    struct FluidsDef {
        std::string name;
        float viscosity;
        float diffusion;
        Vec2 acceleration;
        float buoyancy;
        float dissipation;
        float movekick;
        float movedensity;
        Vec3 movementProjectionX;
        Vec3 movementProjectionY;
        std::array<Vec2, 3> jetKinetics;
        std::array<float, 3> jetKineticsDir;
        std::array<Vec2, 3> jetChaos;
        std::array<float, 3> jetChaosDir;
        std::string initalDensityMapTex;
        float inkFillTime;
        float inkFillRate;
        int32_t renderGridSize;
    };
    extern bool ini_get(Ini const& ini, IniHash h, FluidsDef& val) noexcept;

    struct MaterialOverride {
        std::string subMeshName;
        std::string texture;
        int32_t priority;
        int32_t renderingMode;

        bool load(Ini const& ini, IniHash hash, size_t index) noexcept;
    };

    struct MaterialOverrideList {
        std::array<MaterialOverride, 4> values;
        std::string transMap;
        float transSample;
        int32_t transSource;
        bool load(Ini const& ini, IniHash hash) noexcept;
    };
}


#endif // PARTICLE_FIELDS_H
