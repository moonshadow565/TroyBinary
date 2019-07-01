#include "complex.h"

bool RitoParticle::ComplexParticle::load(const Ini &ini,
                                         const std::string &name,
                                         ComplexEmitter* emitter) noexcept {
    this->emitter = emitter;
    this->name = name;
    IniHash const h = name;

    startFrame = ini[h * "p-startframe"].as_or<int32_t>();

    auto const defnumframes = static_cast<int32_t>(emitter->texDiv.x * emitter->texDiv.y);
    numFrames = ini[h * "p-numframes"].as_or<int32_t>(defnumframes);

    frameRate = ini[h * "p-framerate"].as_or<float>();

    quadType = ini[h * "p-type"].as_or<int32_t>();

    meshFileName = ini[h * "p-mesh"].as_or<std::string>();
    meshSkinMeshFileName = ini[h * "p-skin"].as_or<std::string>();
    meshSkeleton = ini[h * "p-skeleton"].as_or<std::string>();
    meshAnimation = ini[h * "p-animation"].as_or<std::string>();

    if (quadType == 7) {
        projectionYRange = ini[h * "p-projection-y-range"].as_or<float>(5.0f);
        projectionFading = ini[h * "p-projection-fading"].as_or<float>(200.0f);
    }

    uvScrollRate = ini[h * "p-uvscroll-rgb"].as_or<PVec2>();

    velocity = ini[h * "Particle-Velocity"].as_or<PVec3>();

    acceleration = ini[h * "Particle-Acceleration"].as_or<PVec3>();

    worldAcceleration = ini[h * "p-worldaccel"].as_or<PVec3>();

    scale = ini[h * "p-xscale"].as_or<PVec3>(Vec3Inf);

    color = ini[h * "p-xrgba"].as_or<PColor>(ColorFInf);

    bindWeight = ini[h * "p-bindtoemitter"].as_or<PFloat>();

    drag = ini[h * "Particle-Drag"].as_or<PVec3>();

    rotation = ini[h * "p-xquadrot"].as_or<PVec3>();

    rotationIsEnabled = ini[h *  "p-xquadrot-on"].as_or<bool>();

    isDirectionOriented = ini[h * "p-vecalign"].as_or<int32_t>();

    scaleAlongMovementVector = ini[h * "Particle-ScaleAlongMovementVector"].as_or<float>();

    isUniformScale  = ini[h * "uniformscale"].as_or<int32_t>() != 0;

    auto const orient = ini[h * "p-orientation"].as_or<Vec3>();
    if(orient.x != 0.0f || orient.y != 0.0f || orient.z != 0.0f) {
        hasPostRotateOrientation = true;
        // TODO: fix this
        //postRotateOrientation = Mtx44::RotationYawPitchRoll(
        //    orient.y * 0.017453292,
        //    orient.x * 0.017453292,
        //    orient.z * 0.017453292
        //);
    }

    isLocalOrientation = ini[h * "p-local-orient"].as_or<int32_t>() != 0;

    isRandomStartFrame = ini[h * "p-randomstartframe"].as_or<bool>();

    doesCastShadow = ini[h * "p-shadow"].as_or<bool>();

    distortion = ini[h * "p-distortion-power"].as_or<float>();

    uvMode = ini[h * "p-uvmode"].as_or<UvMode>(UvMode::Default);

    if(ini[h * "p-uvscroll-no-alpha"].as_or<bool>()) {
        uvMode = UvMode::LockAlpha;
    }

    trailMode = ini[h * "p-trailmode"].as_or<TrailMode>(TrailMode::Default);

    beamMode = ini[h * "p-beammode"].as_or<BeamMode>(BeamMode::Default);

    return true;
}

bool RitoParticle::ComplexEmitter::load(const Ini &ini,
                                        const std::string &name) noexcept{
    this->name = name;
    IniHash const h = name;
    // TODO: keywords parsing
    this->particle.load(ini, name, this);

    blendMode = ini[h * "rendermode"].as_or<int32_t>();

    renderFlags = 0u;
    if(ini[h * "flag-disable-z"].as_or<bool>()) {
        renderFlags |= 1u;
    }
    if(ini[h * "flag-projected"].as_or<bool>()) {
        renderFlags |= 2u;
    }
    if(ini[h * "teamcolor-correction"].as_or<bool>()) {
        renderFlags |= 4u;
    }
    if(ini[h * "flag-brighter-in-fow"].as_or<bool>()) {
        renderFlags |= 8u;
    }

    pass = ini[h * "pass"].as_or<int32_t>();

    alphaRef = ini[h * "e-alpharef"].as_or<int32_t>(5);

    meshDisableBackfaceCull = ini[h * "p-backfaceon"].as_or<int32_t>() !=0;

    texture = ini[h * "p-texture"].as_or<std::string>();

    texDiv = ini[h * "p-texdiv"].as_or<Vec2>(Vec2Inf);

    particleColorTex = ini[h * "p-rgba"].as_or<std::string>("DefaultColorOverLifetime.dds");

    fallofTex = ini[h * "p-falloff-texture"].as_or<std::string>("DefaultFalloff.dds");

    normalMapTex = ini[h *  "p-normal-map"].as_or<std::string>();

    timeBeforeFirstEmission = ini[h * "e-timeoffset"].as_or<float>();
    if(timeBeforeFirstEmission < 0) {
        timeBeforeFirstEmission = 0.0f;
    }

    lifetime = ini[h * "e-life"].as_or<float>(-1.0f);
    if(lifetime < 0) {
        lifetime = FLT_MAX;
    }

    doesLifetimeScale = ini[h * "e-life-scale"].as_or<bool>();

    timeActiveDuringPeriod = ini[h * "e-active"].as_or<float>(FLT_MAX);
    if(timeActiveDuringPeriod < 0) {
        timeActiveDuringPeriod = FLT_MAX;
    }

    period = ini[h * "e-period"].as_or<float>(FLT_MAX);
    if(period < 0) {
        period = FLT_MAX;
    }

    trailCutoff = ini[h * "e-trail-cutoff"].as_or<float>();

    beamSegments = ini[h * "e-beam-segments"].as_or<int32_t>();

    isSingleParticle = ini[h * "single-particle"].as_or<int32_t>();

    rate = ini[h * "e-rate"].as_or<PFloat>();
    flexRate = ini[h * "e-rate"].as<FlexPFloat>();

    particleLifetime = ini[h * "p-life"].as_or<PFloat>(3.0f);
    flexParticleLifetime = ini[h * "p-life"].as<FlexPFloat>();

    doesParticleLifetimeScale = ini[h * "p-life-scale"].as_or<bool>();

    rateByVelocityFunction = ini[h * "e-ratebyvel"].as_or<PVec2>();

    distortion = ini[h * "p-distortion-power"].as_or<float>();

    distortionMode = ini[h * "p-distortion-mode"].as_or<int32_t>(1);

    birthTranslation = ini[h * "p-postoffset"].as_or<PVec3>();
    flexBirthTranslation = ini[h * "p-postoffset"].as<FlexPVec3>();

    birthRotation = ini[h * "p-quadrot"].as_or<PVec3>();

    birthScale = ini[h * "p-scale"].as_or<PVec3>(Vec3{1.0f, 1.0f, 1.0f});

    birthColor = ini[h * "e-rgba"].as_or<PColor>(ColorF{1.0f, 1.0f, 1.0f, 1.0f});

    birthVelocity = ini[h * "p-vel"].as_or<PVec3>();

    birthAcceleration = ini[h * "p-accel"].as_or<PVec3>();

    birthRotationalVelocity = ini[h * "p-rotvel"].as_or<PVec3>();
    flexBirthRotationalVelocity = ini[h *"p-rotvel"].as<FlexPVec3>();

    birthRotationalAcceleration = ini[h * "Emitter-BirthRotationalAcceleration"].as_or<PVec3>();

    birthDrag = ini[h * "p-drag"].as_or<PVec3>();

    birthOrbitalVelocity = ini[h * "p-orbitvel"].as_or<PVec3>();

    birthFrameRate = ini[h * "e-framerate"].as_or<PFloat>(1.0f);

    isLocalOrientation = ini[h * "e-local-orient"].as_or<int32_t>(1) != 0;

    emitOffset = ini[h * "p-offset"].as_or<PVec3>();

    for(size_t i = 1; i < 10; i++) {
        std::string key = "e-rotation" + std::to_string(i);
        if(auto value = ini[h * key].as<PFloat>(); value) {
            emitRotationAngles.push_back(*value);
            key.append("-axis");
            auto axis = ini[h * key].as_or<Vec3>();
            emitRotationAxes.push_back(axis);
        } else {
            break;
        }
    }

    auto clookup = ini[h * "p-colortype"].as_or<std::string>("1 0");
    sscanf_s(clookup.c_str(), "%d %d", &colorLookupTypes[0], &colorLookupTypes[1]);

    colorLookUpScales = ini[h * "p-colorscale"].as_or<Vec2>(Vec2Inf);

    colorLookUpOffsets = ini[h * "p-coloroffset"].as_or<Vec2>(Vec2Inf);

    auto const lMax = lifetime + 10.0f;

    particleLinger = ini[h * "p-linger"].as_or<float>(lMax);
    if(particleLinger < 0.0f) {
        particleLinger = lMax;
    } else if(particleLinger > lMax) {
        particleLinger =lMax;
    }

    emitterLinger = ini[h * "e-linger"].as_or<float>();
    if(emitterLinger < 0.0f) {
        emitterLinger = lMax;
    } else if(emitterLinger > lMax) {
        emitterLinger = lMax;
    }

    scaleEmitOffsetByBoundObjectSize = ini[h * "p-flexoffset"].as_or<float>();
    scaleBirthScaleByBoundObjectSize = ini[h * "p-flexscale"].as_or<float>();

    scaleEmitOffsetByBoundObjectHeight = ini[h * "p-offsetbyheight"].as_or<float>();
    scaleBirthScaleByBoundObjectHeight = ini[h * "p-scalebyheight"].as_or<float>();

    scaleEmitOffsetByBoundObjectRadius = ini[h * "p-offsetbyradius"].as_or<float>();
    scaleBirthScaleByBoundObjectRadius = ini[h * "p-scalebyradius"].as_or<float>();

    flexScaleEmitOffset = ini[h * "p-scaleEmitOffset"].as<FlexFloat>();
    flexScaleBirthScale = ini[h * "p-scale"].as<FlexFloat>();

    flexOffset = ini[h * "p-offset"].as<FlexPVec3>();

    birthTilingSize = ini[h * "e-tilesize"].as_or<PVec3>();

    birthUVOffset = ini[h * "e-uvoffset"].as_or<PVec2>();
    flexBirthUVOffset = ini[h * "e-uvoffset"].as<FlexPVec2>();

    uvScroll = ini[h * "e-uvscroll"].as_or<Vec2>();

    materialOverrides.load(ini, h);

    soundOnCreateName = ini[h * "SoundOnCreate"].as_or<std::string>();

    soundPersistentName = ini[h * "SoundPersistent"].as_or<std::string>();

    fluid = ini[h * "fluid-params"].as<FluidsDef>();
    return true;
}
