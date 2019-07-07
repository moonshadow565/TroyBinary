#ifndef RITOSKL_HPP
#define RITOSKL_HPP
#include "file.hpp"
#include "types.hpp"

struct RitoSKL {
    struct Bone {
        char boneName[32];
        int32_t parentId;
        float boneLength;
        std::array<float, 4> col0;
        std::array<float, 4> col1;
        std::array<float, 4> col2;
        // This never gets read and is instead set to this
        static constexpr inline std::array<float, 4> col3 = {
            0.f, 0.f, 0.f, 1.f
        };
    };

    uint32_t skeletonID;
    uint32_t numBones;
    std::vector<Bone> bones;
    uint32_t numShaderBones;
    std::vector<uint32_t> shaderBones;

    int load(File const& file) noexcept {
        std::array<char, 8> magic;
        uint32_t version;

        if(!file.read(magic) || !file.read(version)) {
            return -1;
        }
        if(magic != std::array{'r', '3', 'd', '2', 's', 'k', 'l', 't'} ||
                (version != 1 && version != 2)) {
            return -2;
        }

        if(!file.read(skeletonID)) {
            return -4;
        }
        if(!file.read(numBones)) {
            return -5;
        }
        if(!file.read(bones, numBones)) {
            return -6;
        }
        if(version == 2) {
            if(!file.read(numShaderBones)) {
                return -7;
            }
            if(!file.read(shaderBones, numShaderBones)) {
                return -8;
            }
        }
        return 0;
    }
};

#endif // RITOSKL_HPP
