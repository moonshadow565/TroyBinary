#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include "simple.h"
#include "complex.h"
#include "fields.h"
#include <variant>

namespace RitoParticle {
    struct System;
    struct Part {
        std::variant<SimpleParticle, ComplexEmitter> definition;
        // importance level (probably for ordering? or for LOD)
        Importance importance = Importance::Low;
        Vec3 translation = {0.f, 0.f, 0.f};
        Vec3 rotation = {0.f, 0.f, 0.f};
        Vec3 scale = {1.f, 1.f, 1.f};
    };

    struct System {
        std::vector<Part> parts;

        float visibilityRadius;
        std::string soundOnCreateDefault;
        std::string soundPersistentDefault;
        uint32_t flags;
        float buildUpTime;
        MaterialOverrideList materialOverrideList;
        float selfIllumination;

        Importance importance;

        bool load(Ini const& ini) noexcept;
    };
}

#endif // PARTICLE_SYSTEM_H
