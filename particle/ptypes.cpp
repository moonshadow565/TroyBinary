#include "ptypes.h"
#include <cstdio>

namespace RitoParticle {
bool ini_get(const Ini &ini, IniHash h, PTable &val) noexcept {
    if(auto const r = ini[h].as<float>(); r) {
        val.value = *r;
        return true;
    }

    std::vector<PTable::ValueTime> tvpairs;
    for(size_t i = 1; i <= 9; i++) {
        if(auto r = ini[h + std::to_string(i)].as<Vec2>(); r) {
            tvpairs.push_back({r->x, r->y});
        } else {
            break;
        }
    }

    if(tvpairs.size() > 0) {
        if(tvpairs.size() == 2) {
            auto [value0, time0] = tvpairs[0];
            auto [value1, time1] = tvpairs[1];
            if (time1 >= 0.99999899f && time1 < 1.000001f &&
                time0 >= -0.000001f  && time0 < 0.000001f) {
                val.value = PTable::FlatLine {  value0, value1 - value0 };
                return true;
            }
        }
        val.value = std::move(tvpairs);
        return true;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, PFloat &val) noexcept {
    bool good = false;
    if(auto const base_str = ini[h].as<std::string>(); base_str) {
        if(float base_value = {}; sscanf_s(base_str->c_str(), "%f",
                                           &base_value) == 1) {
            good = true;
            val.base = base_value;
        } else {
            return false;
        }
    }

    for(size_t i = 1; i <= 9; i++) {
        if(auto const r = ini[h + std::to_string(i)].as<std::string>(); r) {
            float key_time = {};
            float key_value = {};
            if(sscanf_s(r->c_str(), "%f %f",
                        &key_time, &key_value) != 2) {
                    break;
            }
            val.key_times.push_back(key_time);
            val.key_values.push_back(key_value);
            good = true;
        } else {
            break;
        }
    }

    if (auto p = ini[h + "XP"].as<PTable>(); p) {
        val.ptables[0] = std::move(p);
        good = true;
    } else if (auto p = ini[h + "P"].as<PTable>(); p)  {
        val.ptables[0] = std::move(p);
        good = true;
    } else {}
    return good;
}

bool ini_get(const Ini &ini, IniHash h, PVec2 &val) noexcept {
    bool good = false;
    if(auto const base_str = ini[h].as<std::string>(); base_str) {
        if(Vec2 base_value = {}; sscanf_s(base_str->c_str(), "%f %f",
                                          &base_value.x,
                                          &base_value.y) == 2) {
            good = true;
            val.base = base_value;
        } else {
            return false;
        }
    }

    for(size_t i = 1; i <= 9; i++) {
        if(auto const r = ini[h + std::to_string(i)].as<std::string>(); r) {
            float key_time = {};
            Vec2 key_value = {};
            if(sscanf_s(r->c_str(), "%f %f %f",
                        &key_time,
                        &key_value.x,
                        &key_value.y) != 3) {
                    break;
            }
            val.key_times.push_back(key_time);
            val.key_values.push_back(key_value);
            good = true;
        } else {
            break;
        }
    }

    char const prefixes [2][3] = { "XP", "YP" };
    size_t c = 0;
    for(auto const& prefix: prefixes) {
        if (auto p = ini[h + prefix].as<PTable>(); p) {
            val.ptables[c] = std::move(p);
            good = true;
        }
        c++;
    }
    return good;
}

bool ini_get(const Ini &ini, IniHash h, PVec3 &val) noexcept {
    bool good = false;
    if(auto const base_str = ini[h].as<std::string>(); base_str) {
        if(Vec3 base_value = {}; sscanf_s(base_str->c_str(), "%f %f %f",
                                          &base_value.x,
                                          &base_value.y,
                                          &base_value.z) == 3) {
            good = true;
            val.base = base_value;
        } else {
            return false;
        }
    }

    for(size_t i = 1; i <= 9; i++) {
        if(auto const r = ini[h + std::to_string(i)].as<std::string>(); r) {
            float key_time = {};
            Vec3 key_value = {};
            if(sscanf_s(r->c_str(), "%f %f %f %f",
                        &key_time,
                        &key_value.x,
                        &key_value.y,
                        &key_value.z) != 4) {
                    break;
            }
            val.key_times.push_back(key_time);
            val.key_values.push_back(key_value);
            good = true;
        } else {
            break;
        }
    }

    char const prefixes [3][3] = { "XP", "YP", "ZP"};
    size_t c = 0;
    for(auto const& prefix: prefixes) {
        if (auto p = ini[h + prefix].as<PTable>(); p) {
            val.ptables[c] = std::move(p);
            good = true;
        }
        c++;
    }
    return good;
}

bool ini_get(const Ini &ini, IniHash h, PVec4 &val) noexcept {
    bool good = false;
    if(auto const base_str = ini[h].as<std::string>(); base_str) {
        if(Vec4 base_value = {}; sscanf_s(base_str->c_str(), "%f %f %f %f",
                                          &base_value.x,
                                          &base_value.y,
                                          &base_value.z,
                                          &base_value.w) == 4) {
            good = true;
            val.base = base_value;
        } else {
            return false;
        }
    }

    for(size_t i = 1; i <= 9; i++) {
        if(auto const r = ini[h + std::to_string(i)].as<std::string>(); r) {
            float key_time = {};
            Vec4 key_value = {};
            if(sscanf_s(r->c_str(), "%f %f %f %f %f",
                        &key_time,
                        &key_value.x,
                        &key_value.y,
                        &key_value.z,
                        &key_value.w) != 5) {
                break;
            }
            val.key_times.push_back(key_time);
            val.key_values.push_back(key_value);
            good = true;
        } else {
            break;
        }
    }

    char const prefixes [4][3] = { "XP", "YP", "ZP", "WP"};
    size_t c = 0;
    for(auto const& prefix: prefixes) {
        if (auto p = ini[h + prefix].as<PTable>(); p) {
            val.ptables[c] = std::move(p);
            good = true;
        }
        c++;
    }
    return good;
}

bool ini_get(const Ini &ini, IniHash h, PColor &val) noexcept {
    bool good = false;
    if(auto const base_str = ini[h].as<std::string>(); base_str) {
        if(ColorF base_value = {}; sscanf_s(base_str->c_str(), "%f %f %f %f",
                                          &base_value.r,
                                          &base_value.g,
                                          &base_value.b,
                                          &base_value.a) == 1) {
            good = true;
            val.base = base_value;
        } else {
            return false;
        }
    }

    for(size_t i = 1; i <= 9; i++) {
        if(auto const r = ini[h + std::to_string(i)].as<std::string>(); r) {
            float key_time = {};
            ColorF key_value = {};
            if(sscanf_s(r->c_str(), "%f %f %f %f %f",
                        &key_time,
                        &key_value.r,
                        &key_value.g,
                        &key_value.b,
                        &key_value.a) != 5) {
                break;
            }
            val.key_times.push_back(key_time);
            val.key_values.push_back(key_value);
            good = true;
        } else {
            break;
        }
    }

    char const prefixes [4][3] = { "RP", "GP", "BP", "AP"};
    size_t c = 0;
    for(auto const& prefix: prefixes) {
        if (auto p = ini[h + prefix].as<PTable>(); p) {
            val.ptables[c] = std::move(p);
            good = true;
        }
        c++;
    }
    return good;
}

bool ini_get(const Ini &ini, IniHash h, FlexFloat &val) noexcept {
    char const prefixes[4][7] = {
         "_flex0", "_flex1", "_flex2", "_flex3"
    };
    size_t idx = 0;
    auto const defval = val.value;
    for(auto const prefix: prefixes) {
        if(auto r = ini[h + prefix].as<float>(defval); r) {
            val.value = *r;
            val.idx = idx;
            return true;
        }
        idx++;
    }
    return false;
}

template<typename T>
static inline bool ini_get_flex_p(Ini const& ini, IniHash h, Flex<T>& val) {
    char const prefixes[4][7] = {
         "_flex0", "_flex1", "_flex2", "_flex3"
    };
    size_t idx = 0;
    auto const defval = val.value.eval();
    for(auto const prefix: prefixes) {
        if(auto r = ini[h + prefix].as<T>(defval); r) {
            val.value = *r;
            val.idx = idx;
            return true;
        }
        idx++;
    }
    return false;
}

bool ini_get(const Ini &ini, IniHash h, FlexPFloat &val) noexcept {
    return ini_get_flex_p(ini, h, val);
}

bool ini_get(const Ini &ini, IniHash h, FlexPVec2 &val) noexcept {
    return ini_get_flex_p(ini, h, val);
}

bool ini_get(const Ini &ini, IniHash h, FlexPVec3 &val) noexcept {
    return ini_get_flex_p(ini, h, val);
}

bool ini_get(const Ini &ini, IniHash h, FlexPVec4 &val) noexcept {
    return ini_get_flex_p(ini, h, val);
}

bool ini_get(const Ini &ini, IniHash h, FlexPColor &val) noexcept {
    return ini_get_flex_p(ini, h, val);
}


}
