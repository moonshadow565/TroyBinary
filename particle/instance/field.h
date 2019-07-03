#ifndef FIELD_H
#define FIELD_H
#include "../fields.h"

namespace RitoParticle {
    struct FieldAccelerationInstance {
        FieldAcceleration* definition;
        Vec3 accelerationProbability;
        Vec3 currentAcceleration;
        FieldAccelerationInstance(FieldAcceleration * def)
            : definition(def),
              accelerationProbability(Vec3Inf),
              currentAcceleration({})
        {
            accelerationProbability = def->acceleration.apply_probability(Vec3Inf, std::nullopt);
        }
    };

    struct FieldAttractionInstance {
        FieldAttraction* definition;
        Vec3 positionProbability;
        Vec3 currentPosition;
        float accelerationProbability;
        float currentAcceleration;
        float radiusProbability;
        float currentRadius;
    };

    struct FieldDragInstance {
        FieldDrag* definition;
        Vec3 positionProbability;
        Vec3 currentPosition;
        float strengthProbability;
        float currentStrength;
        float radiusProbability;
        float currentRadius;
    };

    struct FieldNoiseInstance {
        FieldNoise* definition;
        Vec3 positionProbability;
        Vec3 currentPosition;
        float radiusProbability;
        float currentRadius;
        float periodProbability;
        float currentPeriod;
        float currentVelocityDelta;
        Vec3 currentAxisFraction;
        float lastPulseTime;
        uint32_t numPulsesSinceLastEval;
    };

    struct FieldObitalInstance {
        FieldOrbital* definition;
        Vec3 directionProbability;
        Vec3 currentDirection;
    };

    struct FluidsInstance {
        FluidsDef* definition;
        // textures
        Vec3 appliedVelocity;
        Vec2 lastAppliedForce;
        float updateDelta;
        float totalLifetime;
        bool textSetup;
        bool dirty;
        bool valid;
    };
}


#endif // FIELD_H
