#ifndef RITO_PARTICLE_INSTANCE_FIELD_H
#define RITO_PARTICLE_INSTANCE_FIELD_H
#include "../fields.h"

namespace RitoParticle {
    struct FieldAccelerationInstance {
        FieldAcceleration const* definition;
        Vec3 accelerationProbability;
        Vec3 currentAcceleration;

        FieldAccelerationInstance(FieldAcceleration const* def)
            : definition(def),
              accelerationProbability({1.f, 1.f, 1.f}),
              currentAcceleration({})
        {
            accelerationProbability = def->acceleration.apply_probability(Vec3Inf, std::nullopt);
        }

        void eval(float fraction, Mtx44 worldMatrix) {
            auto const anim = definition->acceleration.eval_anim(fraction);
            currentAcceleration = anim * accelerationProbability;
            if(definition->isLocalSpace) {
                currentAcceleration = TransformNormal(currentAcceleration, worldMatrix);
            }
        }
    };

    struct FieldAttractionInstance {
        FieldAttraction const* definition;
        Vec3 positionProbability;
        Vec3 currentPosition;
        float accelerationProbability;
        float currentAcceleration;
        float radiusProbability;
        float currentRadius;

        FieldAttractionInstance(FieldAttraction const* def)
            : definition(def),
              positionProbability({1.f, 1.f, 1.f}),
              currentPosition({}),
              accelerationProbability(1.f),
              currentAcceleration(0.0f),
              radiusProbability(1.f),
              currentRadius(0.0f)
        {
            positionProbability = definition->position.apply_probability(positionProbability);
            accelerationProbability = definition->acceleration.apply_probability(accelerationProbability);
            radiusProbability = definition->radius.apply_probability(radiusProbability);
        }

        void eval(float fraction, Mtx44 worldMatrix) {
            auto const pos = definition->position.eval_anim(fraction) * positionProbability;
            currentPosition = {
                pos.x + worldMatrix[3][0],
                pos.y + worldMatrix[3][1],
                pos.z + worldMatrix[3][2],
            };
            currentAcceleration = definition->acceleration.eval_anim(fraction) * accelerationProbability;
            currentRadius = definition->radius.eval_anim(fraction) * radiusProbability;
        }
    };

    struct FieldDragInstance {
        FieldDrag const* definition;
        Vec3 positionProbability;
        Vec3 currentPosition;
        float strengthProbability;
        float currentStrength;
        float radiusProbability;
        float currentRadius;

        FieldDragInstance(FieldDrag const* def)
            : definition(def),
              positionProbability({1.f, 1.f, 1.f}),
              currentPosition({}),
              strengthProbability(1.f),
              currentStrength(0.0f),
              radiusProbability(1.f),
              currentRadius(0.0f)
        {
            positionProbability = definition->position.apply_probability(positionProbability);
            strengthProbability = definition->strength.apply_probability(strengthProbability);
            radiusProbability = definition->radius.apply_probability(radiusProbability);
        }

        void eval(float fraction, Mtx44 worldMatrix) {
            auto const pos = definition->position.eval_anim(fraction) * positionProbability;
            currentPosition = {
                pos.x + worldMatrix[3][0],
                pos.y + worldMatrix[3][1],
                pos.z + worldMatrix[3][2],
            };
            currentStrength = definition->strength.eval_anim(fraction) * strengthProbability;
            currentRadius = definition->radius.eval_anim(fraction) * radiusProbability;
        }
    };

    struct FieldNoiseInstance {
        FieldNoise const* definition;
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

        FieldNoiseInstance(FieldNoise const* def)
            : definition(def),
              positionProbability({1.f, 1.f, 1.f}),
              currentPosition({}),
              radiusProbability(1.f),
              currentRadius(0.f),
              periodProbability(1.f),
              currentPeriod(INFINITY),
              currentVelocityDelta(0.f),
              currentAxisFraction(def->axisFraction),
              lastPulseTime(INFINITY),
              numPulsesSinceLastEval(0)
        {
            positionProbability = definition->position.apply_probability(positionProbability);
            radiusProbability = definition->radius.apply_probability(radiusProbability);
            periodProbability = definition->period.apply_probability(periodProbability);
        }

        void eval(float fraction, Mtx44 worldMatrix, float currentTime) {
            auto const pos = definition->position.eval_anim(fraction) * positionProbability;
            currentPosition = {
                pos.x + worldMatrix[3][0],
                pos.y + worldMatrix[3][1],
                pos.z + worldMatrix[3][2],
            };
            currentRadius = definition->radius.eval_anim(fraction) * radiusProbability;
            currentPeriod = definition->period.eval_anim(fraction) * periodProbability;
            currentVelocityDelta = definition->velocityDelta.eval(fraction);
            // TODO: float comparison
            if(lastPulseTime == INFINITY) {
                numPulsesSinceLastEval = 1;
                lastPulseTime = currentTime;
            } else {
                auto const tmp1 = lastPulseTime / currentPeriod;
                auto const tmp2 = currentTime / currentPeriod;
                numPulsesSinceLastEval = tmp2 - tmp1;
                if(tmp2 != tmp1) {
                    lastPulseTime = currentTime;
                }
            }
        }
    };

    struct FieldObitalInstance {
        FieldOrbital const* definition;
        Vec3 directionProbability;
        Vec3 currentDirection;

        FieldObitalInstance(FieldOrbital const* def)
            : definition(def),
              directionProbability({1.f, 1.f, 1.f}),
              currentDirection({})
        {
            directionProbability = definition->direction.apply_probability(directionProbability);
        }

        void eval(float fraction, Mtx44 worldMatrix) {
            auto dir = definition->direction.eval_anim(fraction) * directionProbability;
            if(definition->isLocalSpace) {
                dir = TransformNormal(dir, worldMatrix);
            }
            auto const dinvsqrt = 1.0f / std::sqrtf(dir.y * dir.y + dir.x * dir.x + dir.z * dir.z);
            currentDirection = {
                dir.x * dinvsqrt,
                dir.y * dinvsqrt,
                dir.z * dinvsqrt,
            };
        }
    };

    struct FluidsInstance {
        FluidsDef const* definition;
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


#endif // RITO_PARTICLE_INSTANCE_FIELD_H
