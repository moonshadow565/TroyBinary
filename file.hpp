#pragma once
#include <cstdio>
#include <vector>
#include <optional>

struct File {
    FILE* file;
    size_t end;

    inline File(FILE* file) noexcept : file(file), end(0) {
        end = static_cast<size_t>(fseek(file, 0, SEEK_END));
        fseek(file, 0, SEEK_SET);
    }
    File(File && other) noexcept : file(other.file), end(other.end) {
        other.file = nullptr;
        other.end = 0;
    }
    inline ~File() noexcept {
        if(file) {
            fclose(file);
        }
    }
    File(File const&) = delete;
    void operator=(File const&) = delete;
    void operator=(File &&) = delete;

    static std::optional<File> readb(char const* name) noexcept {
        if(FILE* f = nullptr; fopen_s(&f, name, "rb") || !f) {
            return std::nullopt;
        } else {
            return File(f);
        }
    }

    template<typename T>
    inline bool read(T* data, size_t count) const noexcept {
        if(auto const result = fread(data, sizeof(T), count, file);
                result != count) {
            return false;
        }
        return true;
    }

    template<typename T>
    inline bool read(std::vector<T>& value, size_t count) const noexcept {
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
