#ifndef PARTICLE_COMPLEX_H
#define PARTICLE_COMPLEX_H
#include <string>
#include "ptypes.h"
#include "fields.h"
#include "../inibin.h"

namespace RitoParticle {
    struct ComplexEmitter;
    struct ComplexParticle {
        std::string name;
        ComplexEmitter* emitter;
        int32_t startFrame;
        int32_t numFrames;
        float frameRate;
        int32_t quadType;
        std::string meshFileName;
        std::string meshSkinMeshFileName;
        std::string meshSkeleton;
        std::string meshAnimation;
        PVec2 uvScrollRate;
        bool isDirectionOriented;
        float scaleAlongMovementVector;
        bool isUniformScale;
        bool hasPostRotateOrientation;
        Mtx44 postRotateOrientation;
        bool isLocalOrientation;
        bool isRandomStartFrame;
        bool doesCastShadow;
        float distortion;
        UvMode uvMode;
        TrailMode trailMode;
        BeamMode beamMode;
        float projectionYRange;
        float projectionFading;
        PVec3 velocity;
        PVec3 acceleration;
        PVec3 worldAcceleration;
        PVec3 scale;
        PColor color;
        PFloat bindWeight;
        PVec3 drag;
        PVec3 rotation;
        bool rotationIsEnabled;

        bool load(Ini const& ini, std::string const& name,
                  ComplexEmitter* emitter) noexcept;
        inline bool load(Ini const& ini, char const* name,
                         ComplexEmitter* emitter) noexcept {
            return load(ini, std::string{name}, emitter);
        }
    };

    struct ComplexEmitter {
        std::string name;
        ComplexParticle particle;
        int32_t blendMode;
        uint32_t renderFlags;
        int32_t pass;
        int32_t alphaRef;
        bool meshDisableBackfaceCull;
        std::string texture;
        std::string particleColorTex;
        std::string fallofTex;
        std::string normalMapTex;
        Vec2 texDiv;
        PFloat rate;
        PFloat particleLifetime;
        bool doesParticleLifetimeScale;
        float distortion;
        int32_t distortionMode;
        PVec3 birthTranslation;
        PVec3 birthRotation;
        PVec3 birthScale;
        PColor birthColor;
        PVec3 birthVelocity;
        PVec3 birthAcceleration;
        PVec3 birthRotationalVelocity;
        PVec3 birthRotationalAcceleration;
        PVec3 birthDrag;
        PVec3 birthOrbitalVelocity;
        PFloat birthFrameRate;
        PVec3 birthTilingSize;
        PVec2 birthUVOffset;
        PVec2 rateByVelocityFunction;
        Vec2 uvScroll;
        PVec3 emitOffset;
        std::vector<PFloat> emitRotationAngles;
        std::vector<Vec3> emitRotationAxes;
        bool isLocalOrientation;
        float timeBeforeFirstEmission;
        float lifetime;
        bool doesLifetimeScale;
        float period;
        float timeActiveDuringPeriod;
        float trailCutoff;
        int32_t beamSegments;
        int32_t isSingleParticle;
        float particleLinger;
        float emitterLinger;
        std::array<ColorLookupType, 2> colorLookupTypes;
        Vec2 colorLookUpScales;
        Vec2 colorLookUpOffsets;
        float scaleEmitOffsetByBoundObjectSize;
        float scaleBirthScaleByBoundObjectSize;
        float scaleEmitOffsetByBoundObjectHeight;
        float scaleBirthScaleByBoundObjectHeight;
        float scaleEmitOffsetByBoundObjectRadius;
        float scaleBirthScaleByBoundObjectRadius;
        std::string attachmentType;
        std::vector<std::string> excludeAttachmentTypes;
        std::vector<std::string> keywordsExcluded;
        std::vector<std::string> keywordsRequired;
        MaterialOverrideList materialOverrides;
        std::string soundOnCreateName;
        std::string soundPersistentName;
        std::optional<FlexFloat> flexScaleEmitOffset;
        std::optional<FlexFloat> flexScaleBirthScale;
        std::optional<FlexPVec3> flexOffset;
        std::optional<FlexPVec2> flexBirthUVOffset;
        std::optional<FlexPVec3> flexBirthTranslation;
        std::optional<FlexPFloat> flexRate;
        std::optional<FlexPVec3> flexBirthRotationalVelocity;
        std::optional<FlexPFloat> flexParticleLifetime;
        // FlexPFloat flexBirthVelocity; CHECKME: never loaded
        std::optional<FluidsDef> fluid;

        Vec3 overrideTranslation;
        Vec3 overrideRotation;
        Vec3 overrideScale;
        Importance importance;
        bool load(Ini const& ini,
                  std::string const& name) noexcept;

    };
}


#endif // PARTICLE_COMPLEX_H
