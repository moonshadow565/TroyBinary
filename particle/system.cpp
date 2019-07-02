#include "system.h"

bool RitoParticle::System::load(const Ini &ini) noexcept {
    IniHash const h = "System";
    // no real limit is enforced but sprintf's would stop working at some point
    for(size_t i = 1; i < 100; i++) {
        char buffer[64];
        sprintf_s(buffer, sizeof(buffer), "GroupPart%u", i);
        if(auto name = ini[h * buffer].as<std::string>(); name) {
            sprintf_s(buffer, sizeof(buffer), "GroupPart%uImportance", i);
            auto const importancestr = ini[h * buffer].as_or<std::string>();
            Importance importance = Importance::High;
            if(importancestr == "High") {
                importance = Importance::High;
            } else if(importancestr == "Medium") {
                importance = Importance::Normal;
            } else if(importancestr == "Low") {
                importance = Importance::Low;
            } else {
                importance = Importance::NotWhenHigh;
            }

            sprintf_s(buffer, sizeof(buffer), "GroupPart%uType", i);
            auto const type = ini[h * buffer].as_or<std::string>("Complex");
            if(type == "Simple") {
                auto& value = simple.emplace_back();
                value.importance = importance;

                value.load(ini, name.value());
            } else {
                auto& value = complex.emplace_back();
                value.importance = importance;

                value.load(ini, name.value());

                sprintf_s(buffer, sizeof(buffer), "Override-Offset%u", i);
                value.overrideTranslation = ini[h * buffer].as_or<Vec3>();

                sprintf_s(buffer, sizeof(buffer), "Override-Rotation%u", i);
                value.overrideRotation = ini[h * buffer].as_or<Vec3>();

                sprintf_s(buffer, sizeof(buffer), "Override-Scale%u", i);
                value.overrideScale = ini[h * buffer].as_or<Vec3>();
            }
        } else {
            break;
        }
    }

    visibilityRadius = ini[h *  "group-vis"].as_or<float>(250.0f);

    soundOnCreateDefault = ini[h * "SoundOnCreate"].as_or<std::string>();

    soundPersistentDefault = ini[h * "SoundPersistent"].as_or<std::string>();

    if(ini[h * "SimulateWhileOffScreen"].as_or<bool>()) {
      flags |= 1u;
    }
    if(ini[h * "PersistThruDeath"].as_or<bool>()) {
      flags |= 2u;
    }
    if(ini[h * "SimulateOncePerFrame"].as_or<bool>()) {
      flags |= 4u;
    }
    if(ini[h * "SoundEndsOnEmitterEnd"].as_or<bool>()) {
      flags |= 0x08;
    }
    if(ini[h * "SoundsPlayWhileOffScreen"].as_or<bool>()) {
      flags |= 0x10u;
    }
    if(ini[h *  "SimulateEveryFrame"].as_or<bool>()) {
      flags |= 0x20u;
    }
    if(ini[h * "KeepOrientationAfterSpellCast"].as_or<bool>(true)) {
      flags |= 0x40u;
    }

    buildUpTime = ini[h * "build-up-time"].as_or<float>();

    materialOverrideList.load(ini, h);

    selfIllumination = ini[h * "SelfIllumination"].as_or<float>(-1.0f);
    return true;
}
