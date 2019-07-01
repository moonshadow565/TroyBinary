#ifndef INIBIN_H
#define INIBIN_H
#include <unordered_map>
#include <string>
#include <array>
#include <variant>
#include <optional>
#include <type_traits>
#include "types.hpp"

class IniHash {
private:
    std::uint32_t const value;

    constexpr inline static uint32_t
    lower(char const c) {
        return static_cast<uint32_t>(c >= 'A' && c <='Z' ? c + ('a' - 'A') : c);
    }

    constexpr inline static uint32_t
    inihash(char const* str, uint32_t old = 0) {
        char const c = *str;
        return c ? inihash(str + 1, (old * 65599U) + lower(c)) : old;
    }
public:
    constexpr inline
    IniHash(std::uint32_t v) noexcept
        : value(v) {}

    constexpr inline IniHash(char const* str) noexcept
        : value(inihash(str, 0)) {}

    constexpr inline IniHash(std::string const& str) noexcept
        : IniHash(str.c_str()) {}

    constexpr inline IniHash(uint32_t old, char const* str) noexcept
        : value(inihash(str, old)) {}

    constexpr inline IniHash(uint32_t old, std::string const& str) noexcept
        : value(inihash(str.c_str(), old)) {}

    constexpr inline IniHash(char const* section, const char* name) noexcept
        : value(inihash(name, inihash("*", inihash(section)))) {}

    inline IniHash(char const* section, std::string const& name) noexcept
        : IniHash(section, name.c_str()) {}

    inline IniHash(std::string const& section, std::string const& name) noexcept
        : IniHash(section.c_str(), name.c_str()) {}

    inline IniHash(std::string const& section, const char* name) noexcept
        : IniHash(section.c_str(), name) {}

    constexpr inline operator uint32_t() const noexcept {
        return value;
    }
};

inline IniHash operator+(IniHash const left, char const* right) {
    return IniHash(left, right);
}

inline IniHash operator+(IniHash const left, std::string const& right) {
    return IniHash(left, right.c_str());
}

inline IniHash operator*(IniHash const left, char const* right) {
    return IniHash(IniHash(left, "*"), right);
}

inline IniHash operator*(IniHash const left, std::string const& right) {
    return IniHash(IniHash(left, "*"), right.c_str());
}

template<>
struct std::hash<IniHash> : public std::hash<uint32_t> {};

class Ini {
public:
    using IniValue = std::variant<std::nullptr_t,
                                 int32_t,
                                 float,
                                 std::array<float, 2>,
                                 std::array<float, 3>,
                                 std::array<float, 4>,
                                 std::string>;
    using IniMap = std::unordered_map<IniHash, IniValue>;
private:
    IniMap values;

    class ConstReference {
    private:
        Ini const& ini;
        IniHash const hash;
    public:
        ConstReference(Ini const& ini, IniHash h) noexcept
            : ini(ini), hash(h) {}
        ConstReference(ConstReference const&) = delete;
        ConstReference(ConstReference&&) = delete;
        void operator=(ConstReference const&) = delete;
        void operator=(ConstReference&&) = delete;

        template<typename T, typename...D>
        inline std::optional<T> as(D&&...defv) const noexcept {
            if(T value = {std::forward<D>(defv)...}; ini_get(ini, hash, value)) {
                return value;
            }
            return std::nullopt;
        }

        template<typename T, typename...D>
        inline T as_or(D&&...defval) const noexcept {
            T value = {std::forward<D>(defval)...};
            ini_get(ini, hash, value);
            return value;
        }
    };

    class Reference {
    private:
        Ini& ini;
        IniHash const hash;
    public:
        Reference(Ini& ini, IniHash h) noexcept
            : ini(ini), hash(h) {}
        Reference(Reference const&) = delete;
        Reference(Reference&&) = delete;
        void operator=(Reference const&) = delete;
        void operator=(Reference&&) = delete;

        template<typename T>
        inline void operator=(T&& val) noexcept {
            inibin_set(ini, hash, std::forward<T>(val));
        }

        template<typename T>
        inline void operator=(std::optional<T> const& val) noexcept {
            inibin_set(ini, hash, val);
        }

        template<typename T, typename...D>
        inline std::optional<T> as(D&&...defv) const noexcept {
            if(T value = {std::forward<D>(defv)...}; ini_get(ini, hash, value)) {
                return value;
            }
            return std::nullopt;
        }

        template<typename T, typename...D>
        inline T as_or(D&&...defval) const noexcept {
            T value = {std::forward<D>(defval)...};
            ini_get(ini, hash, value);
            return value;
        }
    };

    class ConstIterator {
    private:
        Ini const& ini;
        IniMap::const_iterator i;
    public:
        ConstIterator(Ini const& ini, IniMap::const_iterator const& i) noexcept
            : ini(ini), i(i) { }
        using difference_type = IniMap::const_iterator::difference_type;
        using reference = ConstReference;
        inline auto operator--() { i--; return *this; }
        inline auto operator--(int) { auto t = *this; i--; return t; }
        inline auto operator++() { i++; return *this; }
        inline auto operator++(int) { auto t = *this; i++; return t; }
        inline auto operator*() const { return ConstReference {ini, i->first}; }
        inline auto operator==(const ConstIterator& r) const { return i != r.i; }
        inline auto operator!=(const ConstIterator& r) const { return i != r.i; }
    };

public:
    int from_file(char const* filename) noexcept;

    auto begin() const& { return ConstIterator{ *this, values.begin() }; }
    auto end() const& { return ConstIterator{ *this, values.end() }; }

    auto cbegin() const& { return ConstIterator{ *this, values.cbegin() }; }
    auto cend() const& { return ConstIterator{ *this, values.cend() }; }

    inline Reference operator[](IniHash h) & noexcept {
        return {*this, h};
    }

    inline ConstReference operator[](IniHash h) const& noexcept {
        return {*this, h};
    }

    inline void clear() noexcept { values.clear(); }

    inline void reserve(size_t size) { values.reserve(size); }

    inline size_t size() const noexcept { return values.size(); }

    IniValue get(IniHash h) const noexcept;

    void set(IniHash h, IniValue v) noexcept;
};

extern bool ini_get(Ini const& ini, IniHash h, int32_t& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, bool& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, float& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, std::string& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, std::array<float, 2>& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, std::array<float, 3>& val) noexcept;

extern bool ini_get(Ini const& ini, IniHash h, std::array<float, 4>& val) noexcept;

inline bool ini_get(Ini const& ini, IniHash h, uint32_t& val) noexcept {
    return ini_get(ini, h, reinterpret_cast<int32_t&>(val));
}

inline bool ini_get(Ini const& ini, IniHash h, Vec2& val) noexcept {
    return ini_get(ini, h, reinterpret_cast<std::array<float, 2> &>(val));
}

inline bool ini_get(Ini const& ini, IniHash h, Vec3& val) noexcept {
    return ini_get(ini, h,  reinterpret_cast<std::array<float, 3> &>(val));
}

inline bool ini_get(Ini const& ini, IniHash h, Vec4& val) noexcept {
    return ini_get(ini, h, reinterpret_cast<std::array<float, 4> &>(val));
}

inline bool ini_get(Ini const& ini, IniHash h, ColorF& val) noexcept {
    return ini_get(ini, h, reinterpret_cast<std::array<float, 4> &>(val));
}

template<typename T>
inline std::enable_if_t<std::is_enum_v<T>, bool>
ini_get(Ini const& ini, IniHash h, T& val) noexcept {
    if constexpr(sizeof(T) == sizeof(int32_t)) {
        return ini_get(ini, h, reinterpret_cast<int32_t&>(val));
    } else {
        if(int32_t r = {}; ini_get(ini, h, r)) {
            val = static_cast<T>(r);
            return true;
        }
        return false;
    }
}


#endif // INIBIN_H
