#ifndef RITOSKL_HPP
#define RITOSKL_HPP
#include "file.hpp"
#include "types.hpp"
#include "ritomath.hpp"

struct RitoSKL {
    struct Bone {
        struct Saved {
            std::array<char, 32> boneName;
            int32_t parentId;
            float boneLength;
            std::array<float, 4> col0;
            std::array<float, 4> col1;
            std::array<float, 4> col2;
            static constexpr inline std::array<float, 4> col3 = {
                0.f, 0.f, 0.f, 1.f
            };
        };
        std::array<char, 32> boneName;
        int32_t parentId;
        float boneLength;
        float collisionRadius;
        Mtx44 absPlacement;
        Mtx44 relPlacement;

        Bone() noexcept = default;
        Bone(Bone const&) = default;
        Bone(Bone&&) noexcept = default;
        Bone& operator=(Bone const&) = default;
        Bone& operator=(Bone&&) noexcept = default;

        Bone(Saved const& other) noexcept
            : boneName(other.boneName),
              parentId(other.parentId),
              boneLength(other.boneLength),
              collisionRadius(2.1f),
              absPlacement(Mtx44 {{
                { other.col0[0], other.col1[0], other.col2[0], other.col3[0] },
                { other.col0[1], other.col1[1], other.col2[1], other.col3[1] },
                { other.col0[2], other.col1[2], other.col2[2], other.col3[2] },
                { other.col0[3], other.col1[3], other.col2[3], other.col3[3] }
              }}),
              relPlacement({}){
        }
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
        std::vector<Bone::Saved> savedBones;
        if(!file.read(savedBones, numBones)) {
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
        bones = { savedBones.begin(), savedBones.end() };
        for(auto& bone: bones) {
            if(bone.parentId != -1) {
                auto& parent = bones[static_cast<size_t>(bone.parentId)];
                auto const invParentMtx = Mtx44_inverse(parent.absPlacement);
                bone.relPlacement = Mtx44_Multiply(bone.absPlacement, invParentMtx);
            } else {
                bone.relPlacement = bone.absPlacement;
            }
        }
        return 0;
    }
};

#endif // RITOSKL_HPP
