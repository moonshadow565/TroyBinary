#include "inibin.h"
#include <cstdio>
#include <type_traits>

using IniMap = Ini::IniMap;

namespace {
struct File {
    FILE* file = nullptr;
    size_t end = 0;

    inline ~File() {
        if(file) {
            fclose(file);
        }
    }

    // Don't want to copy this by accident and don't need full wrapper
    File(File const&) = delete;
    File(File &&) = delete;
    void operator=(File const&) = delete;
    void operator=(File &&) = delete;


    template<typename T>
    bool read(T* data, size_t count) const noexcept {
        if(auto const result = fread(data, sizeof(T), count, file);
                result != count) {
            return false;
        }
        return true;
    }

    template<typename T>
    bool read(std::vector<T>& value, size_t count) const noexcept {
        auto const cur = static_cast<size_t>(ftell(file));
        auto const vec_size = count * sizeof(T);
        if(vec_size > (end - cur)) {
            return false;
        }
        value.resize(count);
        return read(&value[0], count);
    }

    template<typename T>
    inline bool read(T& value) const noexcept {
        return read(&value, 1);
    }
};

template<typename T, bool mult = false>
int read_numbers(File const& file, IniMap& ini) noexcept {
    uint16_t count;
    std::vector<uint32_t> hashes;
    std::vector<T> values;
    if(!file.read(count)) {
        return -1;
    }
    if(!file.read(hashes, count)) {
        return -2;
    }
    if(!file.read(values, count)) {
        return -3;
    }
    for(uint32_t i = 0; i < count; i++) {
        if constexpr(mult) {
            ini.try_emplace(hashes[i], values[i] / 10.0f);
        } else {
            ini.try_emplace(hashes[i], values[i]);
        }
    }
    return 0;
}

template<typename T, size_t size, bool mult = false>
int read_array(File const& file, IniMap& ini) noexcept {
    uint16_t count;
    std::vector<uint32_t> hashes;
    std::vector<std::array<T, size>> values;
    if(!file.read(count)) {
        return -1;
    }
    if(!file.read(hashes, count)) {
        return -2;
    }
    if(!file.read(values, count)) {
        return -3;
    }
    for(uint32_t i = 0; i < count; i++) {
        if constexpr(mult) {
            std::array<float, size> result;
            for(size_t c = 0; c < size; c++) {
                result[c] = values[i][c] / 10.0f;
            }
            ini.try_emplace(hashes[i], result);
        } else {
            ini.try_emplace(hashes[i], values[i]);
        }
    }
    return 0;
}

int read_bools(File const& file, IniMap& ini) noexcept {
    uint16_t count = 0;
    std::vector<uint32_t> hashes;
    std::vector<uint8_t> data;
    if(!file.read(count)) {
        return -1;
    }
    if(!file.read(hashes, count)) {
        return -2;
    }
    size_t size = (count / 8) + (count % 8 ? 1 : 0);
    if(!file.read(data, size)) {
        return -3;
    }
    for(uint32_t i = 0; i < count; i++) {
        ini.try_emplace(hashes[i], int32_t((data[i / 8] >> (i % 8)) & 1U));
    }
    return 0;
}

int read_strings(File const& file, IniMap& ini,
                        size_t data_size) noexcept {
    uint16_t count = 0;
    std::vector<uint32_t> hashes;
    std::vector<uint16_t> offsets;
    std::vector<char> data;
    if(!file.read(count)) {
        return -1;
    }
    if(!file.read(hashes, count)) {
        return -2;
    }
    if(!file.read(offsets, count)) {
        return -3;
    }
    if(!file.read(data, data_size)) {
        return -4;
    }
    data.push_back('\0');

    for(uint32_t i = 0; i < count; i++) {
        auto const off = offsets[i];
        if(off >= data.size()) {
            return -5;
        }
        ini.try_emplace(hashes[i], std::string(&data[off]));
    }
    return 0;
}

inline int read_v2(const char *filename, IniMap& ini) noexcept {
    File file = {};
    if(fopen_s(&file.file, filename, "rb")) {
        return -1;
    }
    file.end = static_cast<size_t>(fseek(file.file, 0, SEEK_END));
    fseek(file.file, 0, SEEK_SET);

    uint8_t version;
    if(!file.read(version)) {
        return -2;
    }
    if(version != 2) {
        return -3;
    }
    uint16_t strings_data_length;
    if(!file.read(strings_data_length)) {
        return -4;
    }
    uint16_t flags;
    if(!file.read(flags)) {
        return -5;
    }
    if(flags & (1 << 0)) {
        if(auto r = read_numbers<int32_t>(file, ini); r) {
            return r - 10;
        }
    }
    if(flags & (1 << 1)) {
        if(auto r = read_numbers<float>(file, ini); r) {
            return r - 20;
        }
    }
    if(flags & (1 << 2)) {
        if(auto r = read_numbers<uint8_t, true>(file, ini); r) {
            return r - 30;
        }
    }
    if(flags & (1 << 3)) {
        if(auto r = read_numbers<int16_t>(file, ini); r) {
            return r - 40;
        }
    }
    if(flags & (1 << 4)) {
        if(auto r = read_numbers<uint8_t>(file, ini); r) {
            return r - 50;
        }
    }
    if(flags & (1 << 5)) {
        if(auto r = read_bools(file, ini); r) {
            return r - 60;
        }
    }
    if(flags & (1 << 6)) {
        if(auto r = read_array<uint8_t, 3, true>(file, ini); r) {
            return r - 70;
        }
    }
    if(flags & (1 << 7)) {
        if(auto r = read_array<float, 3>(file, ini); r) {
            return r - 80;
        }
    }
    if(flags & (1 << 8)) {
        if(auto r = read_array<uint8_t, 2, true>(file, ini); r) {
            return r - 90;
        }
    }
    if(flags & (1 << 9)) {
        if(auto r = read_array<float, 2>(file, ini); r) {
            return r - 100;
        }
    }
    if(flags & (1 << 10)) {
        if(auto r = read_array<uint8_t, 4, true>(file, ini); r) {
            return r - 110;
        }
    }
    if(flags & (1 << 11)) {
        if(auto r = read_array<float, 4>(file, ini); r) {
            return r - 120;
        }
    }
    if(flags & (1 << 12)) {
        if(auto r = read_strings(file, ini, strings_data_length); r) {
            return r - 130;
        }
    }
    return 0;
}
}

int Ini::from_file(const char *filename) noexcept
{
    return read_v2(filename, values);
}

Ini::IniValue Ini::get(IniHash h) const noexcept {
    if(auto r = values.find(h); r != values.end()) {
        return r->second;
    }
    return nullptr;
}

void Ini::set(IniHash h, Ini::IniValue v) noexcept {
    if(std::holds_alternative<std::nullptr_t>(v)) {
        values.erase(h);
    } else {
        values[h] = v;
    }
}

bool ini_get(const Ini &ini, IniHash h, int32_t &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, float>) {
            val = static_cast<int32_t>(value);
            return true;
        } else if constexpr(std::is_same_v<T, int32_t>) {
            val = value;
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            val= atoi(value.c_str());
            return true;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, bool &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, float>) {
            val = value != 0.0f;
            return true;
        } else if constexpr(std::is_same_v<T, int32_t>) {
            val = value != 0;
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            val = value[0] == '1';
            return true;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, float &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, float>) {
            val =  value;
            return true;
        } else if constexpr(std::is_same_v<T, int32_t>) {
            val =  static_cast<float>(value);
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            val = static_cast<float>(atof(value.c_str()));
            return true;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, std::string &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, std::array<float, 2>>) {
            char buffer[128] = {};
            auto const size = snprintf(buffer, sizeof(buffer),
                                       "%f %f",
                                       static_cast<double>(value[0]),
                                       static_cast<double>(value[1]));
            if(size < 0) {
                return false;
            }
            val = std::string(buffer, static_cast<size_t>(size));
            return true;
        } else if constexpr(std::is_same_v<T, std::array<float, 3>>) {
            char buffer[128] = {};
            auto const size = snprintf(buffer, sizeof(buffer),
                                       "%f %f %f",
                                       static_cast<double>(value[0]),
                                       static_cast<double>(value[1]),
                                       static_cast<double>(value[2]));
            if(size < 0) {
                return false;
            }
            val = std::string(buffer, static_cast<size_t>(size));
            return true;
        } else if constexpr(std::is_same_v<T, std::array<float, 4>>) {
            char buffer[128] = {};
            auto const size = snprintf(buffer, sizeof(buffer),
                                       "%f %f %f %f",
                                       static_cast<double>(value[0]),
                                       static_cast<double>(value[1]),
                                       static_cast<double>(value[2]),
                                       static_cast<double>(value[3]));
            if(size < 0) {
                return false;
            }
            val = std::string(buffer, static_cast<size_t>(size));
            return true;
        } else if constexpr(std::is_same_v<T, float>) {
            val = std::to_string(value);
            return true;
        } else if constexpr(std::is_same_v<T, int32_t>) {
            val = std::to_string(value);
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            val = value;
            return true;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, std::array<float, 2> &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, std::array<float, 2>>) {
            val = value;
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            std::array<float, 2> result;
            if(sscanf_s(value.c_str(), "%f %f",
                        &result[0],
                        &result[1]) == 2) {
                val = result;
                return true;
            }
            return false;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, std::array<float, 3> &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, std::array<float, 3>>) {
            val = value;
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            std::array<float, 3> result;
            if(sscanf_s(value.c_str(), "%f %f %f",
                        &result[0],
                        &result[1],
                        &result[2]) == 3) {
                val = result;
                return true;
            }
            return false;
        } else {
            return false;
        }
    }, ini.get(h));
}

bool ini_get(const Ini &ini, IniHash h, std::array<float, 4> &val) noexcept {
    return std::visit([&val](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, std::array<float, 4>>) {
            val = value;
            return true;
        } else if constexpr(std::is_same_v<T, std::string>) {
            std::array<float, 4> result;
            if(sscanf_s(value.c_str(), "%f %f %f %f",
                        &result[0],
                        &result[1],
                        &result[2],
                        &result[3]) == 4) {
                val = result;
                return true;
            }
            return false;
        } else {
            return false;
        }
    }, ini.get(h));
}
