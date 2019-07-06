#ifndef PARTICLE_SIMPLE_H
#define PARTICLE_SIMPLE_H
#include <vector>
#include "../inibin.h"
#include "ptypes.h"
#include "fields.h"

namespace RitoParticle {
    struct SimpleParticle;

    struct SimpleEmitter {
        std::string name;                                // name ID of the emitter
        PFloat rate;                                     // number particles emitted per second
        PFloat particleLifetime;                         // lifetime of each particle
        bool doesParticleLifetimeScales;                 // ??? scale with what
        float particleLinger;                            // ??? time a particle stays around without being updated after running out of time
        float emitterLinger;                             // ??? time the emitter stays around without being updated after running out of time

        // particle birth data
        PVec3 birthTranslation;                            // position of new particles
        PFloat birthRotation;                              // rotation of new particles
        PFloat birthScale;                                 // scale of new particles
        PVec3 birthVelocity;                               // directional velocity of new particles
        PFloat birthRotationalVelocity;                    // rotational velocity of new particles

        PVec3 emitOffset;                                  // base offset of the whole emitter
        std::vector<PFloat> emitRotationAngles;            // ??? most likely the angles at which to emit particles from
        std::vector<Vec3> emitRotationAxes;                // ??? most likely the axes at which to emit particles from
        bool isLocalOrientation;                           // is the emitter orientation in local space relative to the system
        bool particleIsLocalOrientation;                   // is particle orientation in local space relative to the emitter

        // particle orbiting
        bool hasFixedOrbit;                                // particle orbits around a fixed orbit
        FixedOrbitType fixedOrbitType;                     // orbit axis (enum)
        Vec2 particleBind;                                 // ??? maybe the position to orbit around?

        bool lockedToEmitter;                              // ??? is the particle Matrix relative to the emitter or the world
        float timeBeforeFirstEmission;                     // emitter sleeps before start for this long
        float lifetime;                                    // lifetime of the emitter
        bool doesLifetimeScale;                            // ??? scale with what
        float period;                                      // ?
        float timeActiveDuringPeriod;                      // ?

        bool doesCastShadow;                               // does the particle mesh cast shadows
        int32_t isSingleParticle;                          // is just a single particle

        // things related to the textures
        int32_t startFrame;                                // frame ID inside the texture (start ID if animated)
        int32_t numFrames;                                 // number of animation frames (1 if not animated)
        float frameRate;                                   // animation framerate (.anm files are 30.0f)
        int32_t quadType;                                  // ?
        Vec2 uvScrollRate;                                 // scroll speed for the UV coordinates
        bool uvScrollClamp;                                // are the scrolling coordinates clamped at the edges (is this for sampler state?)
        bool isDirectionOriented;                          // ??? maybe does orient the particle towards its movement direction

        float scaleAlongMovementVector;                    // extra scale applied in particle movement direction
        PFloat scale;                                      // emitter scale
        PFloat rotation;                                   // emitter rotation
        bool rotationIsEnabled;                            // is emitter rotating

        // are there any particles projected over the scene? with which matrices?
        float projectionYRange;                            // ?
        float projectionFading;                            // ?
        Vec2 scaleBias;                                    // ?

        Orientation orientation;                        // Orients the particles towards camera or an axis
        bool isRandomStartFrame;                        // if true, a random startframe gets picked (only for animations?)
        bool scaleUpFromOrigin;                         // scale the particle from the origin instead of its own center

        // some stats for doing lookup on the color texture
        std::array<ColorLookupType, 2> colorLookUpTypes; // ?
        Vec2 colorLookUpScales;                          // ?
        Vec2 colorLookUpOffsets;                         // ?

        // scaling with "BoundObjectSize", what is that?
        float scaleEmitOffsetByBoundObjectSize;          // ?
        float scaleBirthScaleByBoundObjectSize;          // ?
        float scaleEmitOffsetByBoundObjectHeight;        // ?
        float scaleBirthScaleByBoundObjectHeight;        // ?
        float scaleEmitOffsetByBoundObjectRadius;        // ?
        float scaleBirthScaleByBoundObjectRadius;        // ?

        // data for using 3d meshes
        std::string meshFileName;                        // file name for .sco/.scb static 3d mesh used
        std::string meshSkinMeshFileName;                // file name for .skl animated 3d mesh used
        std::string meshSkeleton;                        // file name for .skn file for animated 3d mesh
        std::string meshAnimation;                       // file name for .anm file for the default animation for the 3d mesh
        bool meshDisableBackfaceCull;                    // disables backface fulling for the mesh

        // unkown
        std::string attachmentTypeString;                       // ?
        std::vector<std::string> ExcludeAttachmentTypes;        // ?
        std::vector<std::string> keywordsExcluded;              // ?
        std::vector<std::string> keywordsRequired;              // ?

        // fields to manipulate particle movement ... needs more investigation on if and how they are used
        std::vector<FieldAcceleration> fieldAccelerationList;   // acceleration fields used
        std::vector<FieldAttraction> fieldAttractionList;       // attraction fields used
        std::vector<FieldDrag> fieldDragList;                   // drag fields used
        std::vector<FieldNoise> fieldNoiseList;                 // noise fields used
        std::vector<FieldOrbital> fieldOrbitalList;             // orbital fields used

        // advanced stuff
        std::optional<FluidsDef> fluid;                         // fuild dynamics in particles this old? what?
        MaterialOverrideList materialOverrides;                 // custom material used, overriding the default one (maybe used for some special effects)

        // sounds
        std::string soundOnCreate;                              // file name for sound to be played on emitter spawn
        std::string soundPersistentName;                        // file name for sound persisting as long as the emitter is alive

        // all the flex fields are duplicates of already existing fields, are they overrides? or something else?
        std::optional<FlexFloat> flexScaleEmitOffset;                          // ?????
        std::optional<FlexFloat> flexScaleBirthScale;                          // ?????
        std::optional<FlexPVec3> flexOffset;                                   // ?????
        std::optional<FlexPVec3> flexBirthTranslation;                         // ?????
        std::optional<FlexPFloat> flexRate;                                    // ?????
        std::optional<FlexPFloat> flexBirthRotationVelocity;                   // ?????
        std::optional<FlexPFloat> flexParticleLifetime;                        // ?????
        std::optional<FlexPVec3> flexBirthVelocity;                            // ?????

        bool load(Ini const& ini, std::string const& name, SimpleParticle* particle) noexcept;
    };

    struct SimpleParticle {
        std::string name;                               // name ID of the particle
        int32_t blendMode;                              // shader blend mode (0 = additive, 1 = normal?)
        uint32_t renderFlags;                           // render flags (not from an enum?)
        int32_t pass;                                   // ??? maybe which render pass the particle goes in? forward vs deferred?
        int32_t alpharef;                               // ???
        std::string texture;                            // base texture
        std::string colorTex;                           // color texture
        std::string normalMapTex;                       // normal texture
        std::string falloffTex;                         // falloff texture (need to read up on what this is good for)
        float distortion;                               // distortion factor for shader
        uint32_t distortionMode;                        // ??? not an enum?
        Vec2 texDiv;                                    // number of textures packed in the base texture in x and y
        std::vector<SimpleEmitter> emitters;            // list of emitters (always 1?)
        UvMode uvmode;                                  // uv coordinates mode (enum)

        bool load(Ini const& ini, std::string const& name) noexcept;
        inline bool load(Ini const& ini, char const* name) noexcept {
            return load(ini, std::string{name});
        }
    };
}



#endif // PARTICLE_SIMPLE_H
