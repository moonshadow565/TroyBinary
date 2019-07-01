#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include "simple.h"
#include "complex.h"
#include "fields.h"
#include <variant>

namespace RitoParticle {
    struct System {
        std::vector<SimpleParticle> simple;
        std::vector<ComplexEmitter> complex;

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
