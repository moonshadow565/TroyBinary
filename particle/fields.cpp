#include "fields.h"

namespace RitoParticle {

bool ini_get(const Ini &ini, IniHash h, FieldAcceleration &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;
        val.isLocalSpace = ini[h * "f-localspace"].as_or<int32_t>() != 0;
        val.acceleration = ini[h * "f-accel"].as_or<PVec3>(Vec3Inf);
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FieldAttraction &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;
        val.position = ini[h * "f-pos"].as_or<PVec3>();
        val.radius = ini[h * "f-radius"].as_or<PFloat>();
        val.acceleration = ini[h * "f-accel"].as_or<PFloat>();
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FieldDrag &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;
        val.position = ini[h * "f-pos"].as_or<PVec3>();
        val.radius = ini[h * "f-radius"].as_or<PFloat>();
        val.strength = ini[h * "f-drag"].as_or<PFloat>();
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FieldNoise &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;
        val.position = ini[h * "f-pos"].as_or<PVec3>();
        val.radius = ini[h * "f-radius"].as_or<PFloat>();
        val.period = ini[h * "f-period"].as_or<PFloat>(FLT_MAX);
        val.velocityDelta = ini[h * "f-veldelta"].as_or<PFloat>();
        val.axisFraction = ini[h * "f-axisfrac"].as_or<Vec3>(Vec3Inf);
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FieldOrbital &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;
        val.isLocalSpace = ini[h * "f-localspace"].as_or<int32_t>() != 0;
        val.direction = ini[h * "f-direction"].as_or<PVec3>(Vec3Inf);
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FluidsDef &val) noexcept {
    if(auto name = ini[h].as<std::string>(); name) {
        IniHash const h = *name;
        val.name = *name;

        val.viscosity = ini[h * "f-viscosity"].as_or<float>();
        val.diffusion = ini[h * "f-diffusion"].as_or<float>();
        val.acceleration = ini[h * "f-accel"].as_or<Vec2>();
        val.buoyancy = ini[h * "f-buoyancy"].as_or<float>();
        val.dissipation = ini[h * "f-dissipation"].as_or<float>();
        val.movekick = ini[h * "f-startkick"].as_or<float>(1.0f);
        val.movedensity = ini[h * "f-denseforce"].as_or<float>();
        val.movementProjectionX = ini[h * "f-movement-x"].as_or<Vec3>();
        val.movementProjectionY = ini[h * "f-movement-y"].as_or<Vec3>();
        char const jet_pos[3][10] = {
            "f-jetpos1", "f-jetpos2", "f-jetpos3",
        };
        char const jet_dir[3][10] = {
            "f-jetdir1", "f-jetdir2", "f-jetdir3",
        };
        char const jet_speed[3][12] ={
            "f-jetspeed1", "f-jetspeed2", "f-jetspeed3",
        };
        char const jet_speeddiff[3][16] ={
            "f-jetspeeddiff1", "f-jetspeeddiff2", "f-jetspeeddiff3",
        };
        for(size_t i = 0; i < 2; i++) {
            val.jetKinetics[i] = ini[h * jet_pos[i]].as_or<Vec2>();
            val.jetKineticsDir[i] = ini[h * jet_dir[i]].as_or<float>();
            val.jetChaos[i] = ini[h * jet_speed[i]].as_or<Vec2>();
            val.jetChaosDir[i] = ini[h * jet_speeddiff[i]].as_or<float>();
        }
        val.initalDensityMapTex = ini[h * "f-initdensity"].as_or<std::string>("");
        val.inkFillTime = ini[h * "f-life"].as_or<float>();
        val.inkFillRate = ini[h * "f-rate"].as_or<float>();
        val.renderGridSize = ini[h * "f-rendersize"].as_or<int32_t>();
        return true;
    }
    return false;
}

bool MaterialOverride::load(const Ini &ini, IniHash h, size_t index) noexcept {
    char buffer[64] = {};
    {
        sprintf_s(buffer, sizeof(buffer), "MaterialOverride%uSubMesh", index);
        subMeshName = ini[h * buffer].as_or<std::string>();
    }
    {
        sprintf_s(buffer, sizeof(buffer), "MaterialOverride%uTexture", index);
        texture = ini[h * buffer].as_or<std::string>();
    }
    {
        sprintf_s(buffer, sizeof(buffer), "MaterialOverride%uPriority", index);
        priority = ini[h * buffer].as_or<int32_t>();
    }
    {
        sprintf_s(buffer, sizeof(buffer), "MaterialOverride%dBlendMode", index);
        renderingMode = ini[h * buffer].as_or<int32_t>();
    }
    return true;
}

bool MaterialOverrideList::load(const Ini &ini, IniHash h) noexcept {
    transMap = ini[h * "MaterialOverrideTransMap"].as_or<std::string>();
    transSample = ini[h * "p-trans-sample"].as_or<float>();
    transSource = ini[h * "MaterialOverrideTransSource"].as_or<int32_t>();
    for(size_t i = 0; i < 4; i++) {
        values[i].load(ini, h, i );
    }
    return true;
}
}

