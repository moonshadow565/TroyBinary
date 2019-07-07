#ifndef RITOSKN_H
#define RITOSKN_H
#include "file.hpp"
#include "types.hpp"

struct RitoSKN {
    struct SubMesh {
        char name[64];
        uint32_t firstVertex;
        uint32_t vertexCount;
        uint32_t firstIndex;
        uint32_t indexCount;
    };

    struct Vertex {
        std::array<float, 3> pos;
        std::array<uint8_t, 4> boneIndices;
        std::array<float, 4> weights;
        std::array<float, 3> normal;
        std::array<float, 2> uv;
    };

    uint32_t magic;
    uint32_t version;
    uint32_t submeshCount;
    std::vector<SubMesh> submeshes;
    uint32_t numIndices;
    uint32_t numVertices;
    std::vector<uint16_t> indexData;
    std::vector<Vertex> vertexData;
    Vec3 pivotPoint;

    int load(File const& file) noexcept {
        if(!file.read(magic) || !file.read(version)) {
            return -1;
        }
        if(magic != 0x00112233 || version < 0x10000u || version > 0x10002u) {
            return -2;
        }
        if(version > 0x10000u) {
            if(!file.read(submeshCount)) {
                return -3;
            }
            if(!file.read(submeshes, submeshCount)) {
                return -4;
            }
        }
        if(!file.read(numIndices)) {
            return -5;
        }
        if(!file.read(numVertices)) {
            return -6;
        }
        if(!file.read(indexData, numIndices)) {
            return -7;
        }
        if(!file.read(vertexData, numVertices)) {
            return -8;
        }
        if(!file.read(pivotPoint)) {
            return -9;
        }
        return 0;
    }
};



#endif // RITOSKN_H
