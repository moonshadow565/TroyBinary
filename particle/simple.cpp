#include "simple.h"
using namespace  RitoParticle;


bool SimpleEmitter::load(const Ini &ini, const std::string &name,
                         SimpleParticle* particle) noexcept {
    this->name = name;
    IniHash const h = name;

    // TODO: keywords stuff

    startFrame = ini[h * "p-startframe"].as_or<int32_t>();

    auto const numFramesDef = static_cast<int32_t>(particle->texDiv.x * particle->texDiv.y);
    numFrames = ini[h * "p-numframes"].as_or<int32_t>(numFramesDef);

    frameRate = ini[h * "p-frameRate"].as_or<float>();

    quadType = ini[h * "p-type"].as_or<int32_t>();

    if(quadType == 3) {
        meshFileName = ini[h * "p-mesh"].as_or<std::string>("arrow01.sco");
        meshSkinMeshFileName = ini[h * "p-skin"].as_or<std::string>();
        meshSkeleton = ini[h * "p-skeleton"].as_or<std::string>();
        meshAnimation = ini[h * "p-animation"].as_or<std::string>();
    }

    if(quadType == 7) {
        projectionYRange = ini[h * "p-projection-y-range"].as_or<float>(5.0f);
        projectionFading = ini[h * "p-projection-fading"].as_or<float>(200.0f);
    }

    meshDisableBackfaceCull = ini[h * "p-backfaceon"].as_or<int32_t>() !=0;

    uvScrollRate = ini[h * "p-uvscroll-rgb"].as_or<Vec2>();

    uvScrollClamp = ini[h * "p-uvscroll-rgb-clamp"].as_or<bool>();

    isDirectionOriented = ini[h * "p-vecalign"].as_or<bool>();

    scaleAlongMovementVector = ini[h * "Particle-ScaleAlongMovementVector"].as_or<float>();
    scale = ini[h * "p-xscale"].as_or<PFloat>(1.0f);

    rotation = ini[h * "p-xquadrot"].as_or<PFloat>();

    rotationIsEnabled = ini[h * "p-xquadrot-on"].as_or<bool>();

    timeBeforeFirstEmission = ini[h * "e-timeoffset"].as_or<float>(0.0f);
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

    doesCastShadow = ini[h * "p-shadow"].as_or<bool>();

    isSingleParticle = ini[h * "single-particle"].as_or<int32_t>();
    if(isSingleParticle == 30) {
        isSingleParticle = 0;
    }

    rate = ini[h * "e-rate"].as_or<PFloat>();
    flexRate = ini[h * "e-rate"].as<FlexPFloat>();

    particleLifetime = ini[h * "p-life"].as_or<PFloat>();
    flexParticleLifetime = ini[h * "p-life"].as<FlexPFloat>();

    doesParticleLifetimeScales = ini[h * "p-life-scale"].as_or<bool>();

    birthTranslation = ini[h * "p-ostoffset"].as_or<PVec3>();
    flexBirthTranslation = ini[h * "p-postoffset"].as<FlexPVec3>();

    birthRotation = ini[h * "p-quadrot"].as_or<PFloat>();

    birthScale = ini[h * "p-scale"].as_or<PFloat>(1.0f);

    birthVelocity = ini[h * "p-vel"].as_or<PVec3>();
    flexBirthVelocity = ini[h * "p-vel"].as<FlexPVec3>();

    birthRotationalVelocity = ini[h * "p-rotvel"].as_or<PFloat>();
    flexBirthRotationVelocity = ini[h * "p-rotvel"].as<FlexPFloat>();

    isLocalOrientation = ini[h * "e-local-orient"].as_or<bool>();

    particleIsLocalOrientation = ini[h * "p-local-orient"].as_or<bool>();

    hasFixedOrbit = ini[h * "p-fixedorbit"].as_or<bool>();

    fixedOrbitType = ini[h * "p-fixedorbittype"].as_or<FixedOrbitType>( FixedOrbitType::WorldY);

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

    for(size_t i = 1; i < 10; i++) {
        std::string key = "field-accel-" + std::to_string(i);
        if(auto field = ini[h * key].as<FieldAcceleration>(); field) {
            fieldAccelerationList.push_back(*field);
        } else {
            break;
        }
    }

    for(size_t i = 1; i < 10; i++) {
        std::string key = "field-attract-" + std::to_string(i);
        if(auto field = ini[h * key].as<FieldAttraction>(); field) {
            fieldAttractionList.push_back(*field);
        } else {
            break;
        }
    }

    for(size_t i = 1; i < 10; i++) {
        std::string key = "field-drag-" + std::to_string(i);
        if(auto field = ini[h * key].as<FieldDrag>(); field) {
            fieldDragList.push_back(*field);
        } else {
            break;
        }
    }

    for(size_t i = 1; i < 10; i++) {
        std::string key = "field-noise-" + std::to_string(i);
        if(auto field = ini[h * key].as<FieldNoise>(); field) {
            fieldNoiseList.push_back(*field);
        } else {
            break;
        }
    }

    for(size_t i = 1; i < 10; i++) {
        std::string key = "field-orbit-" + std::to_string(i);
        if(auto field = ini[h * key].as<FieldOrbital>(); field) {
            fieldOrbitalList.push_back(*field);
        } else {
            break;
        }
    }

    fluid = ini[h * "fluid-params"].as<FluidsDef>();

    particleBind = ini[h * "p-bindtoemitter"].as_or<Vec2>();

    lockedToEmitter = ini[h * "p-lockedtoemitter"].as_or<bool>();

    // isFollowingTerrain = ini[h * "p-followterrain"].as_or<bool>();

    scaleBias = ini[h * "p-scalebias"].as_or<Vec2>(Vec2Inf);

    orientation = ini[h * "p-simpleorient"].as_or<Orientation>(Orientation::Camera);

    isRandomStartFrame = ini[h * "p-randomstartframe"].as_or<bool>();

    scaleUpFromOrigin = ini[h * "p-scaleupfromorigin"].as_or<bool>();

    auto clookup = ini[h * "p-colortype"].as_or<std::string>("1 0");
    sscanf_s(clookup.c_str(), "%d %d",
            &colorLookUpTypes[0], &colorLookUpTypes[1]);

    colorLookUpScales = ini[h * "p-colorscale"].as_or<Vec2>(Vec2Inf);

    colorLookUpOffsets = ini[h * "p-coloroffset"].as_or<Vec2>(Vec2Inf);

    particleLinger = ini[h * "p-linger"].as_or<float>(10.0f);
    if(particleLinger < 0.0f) {
        particleLinger = 10.0f;
    } else if(particleLinger > 10.0f) {
        particleLinger = 10.0f;
    }

    emitterLinger = ini[h * "e-linger"].as_or<float>(10.0f);
    if(emitterLinger < 0.0f) {
        emitterLinger = 10.0f;
    } else if(emitterLinger > 10.0f) {
        emitterLinger = 10.0f;
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

    materialOverrides.load(ini, h);

    soundOnCreate = ini[h * "SoundOnCreate"].as_or<std::string>();

    // voiceOverOnCreate =
    // ini[h * "VoiceOverOnCreate"].as_or<std::string>();

    soundPersistentName = ini[h * "SoundPersistent"].as_or<std::string>();

    // VoiceOverPersistentName =
    // ini[h * "VoiceOverPersistent"].as_or<std::string>();


    return true;
}

bool SimpleParticle::load(Ini const &ini, const std::string &name) noexcept{
    this->name = name;
    IniHash const h = name;

    blendMode = ini[h * "rendermode"].as_or<int32_t>();
    pass = ini[h * "pass"].as_or<int32_t>();
    alpharef = ini[h * "e-alpharef"].as_or<int32_t>(5);
    uvmode = ini[h * "p-uvmode"].as_or<UvMode>(UvMode::Default);
    distortion = ini[h * "p-distortion-power"].as_or<float>();
    distortionMode = ini[h * "p-distortion-mode"].as_or<uint32_t>(1u);
    if(ini[h * "dont-scroll-alpha-UV"].as_or<bool>()) {
        uvmode = UvMode::LockAlpha;
    }
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
    texture = ini[h * "p-texture"].as_or<std::string>();
    texDiv = ini[h * "p-texdiv"].as_or<Vec2>(Vec2Inf);
    colorTex = ini[h * "p-rgba"].as_or<std::string>();
    normalMapTex = ini[h * "p-normal-map"].as_or<std::string>();
    falloffTex = ini[h * "p-falloff-texture"].as_or<std::string>();

    for(size_t i = 1;  i < 10; i++) {
        if(auto e = ini[h * ("Emitter" + std::to_string(i))].as<std::string>(); e) {
            SimpleEmitter emitter{};
            emitter.load(ini, e.value(), this);
            emitters.push_back(std::move(emitter));
        }
    }

    if(emitters.size() == 0) {
        SimpleEmitter emitter{};
        emitter.load(ini, name, this);
        emitters.push_back(std::move(emitter));
    }

    return true;
}
