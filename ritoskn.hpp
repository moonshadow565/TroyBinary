#ifndef RITOSKN_H
#define RITOSKN_H
#include "file.hpp"
#include "types.hpp"

struct RitoSKN {
    struct SubMesh {
        std::array<char, 64> name;
        int32_t firstVertex;
        int32_t vertexCount;
        int32_t firstIndex;
        int32_t indexCount;
    };

    struct VertexBasic {
        Vec3 position;
        std::array<uint8_t, 4> boneIndices;
        std::array<float, 4> weights;
        Vec3 normal;
        Vec2 textureCord;
    };
    struct VertexWithColor {
        Vec3 position;
        std::array<uint8_t, 4> boneIndices;
        std::array<float, 4> weights;
        Vec3 normal;
        Vec2 textureCord;
        ColorB color;
    };
    // Format less than  0 ??
    struct VertexWithTangent {
        Vec3 position;
        std::array<uint8_t, 4> boneIndices;
        std::array<float, 4> weights;
        Vec3 normal;
        Vec2 textureCord;
        Vec4 tangent;
    };

    std::vector<SubMesh> submeshes;
    std::vector<uint16_t> indexData;
    uint32_t flags;
    Box3D boundingBox;
    Sphere boundingSphere;
    std::variant<std::vector<VertexBasic>, std::vector<VertexWithColor>> vertexData;
    std::array<float, 3> pivotPoint;

    int load(File const& file) noexcept {
        uint32_t magic;
        uint32_t version;

        if(!file.read(magic) || !file.read(version)) {
            return -1;
        }
        if(magic != 0x00112233) {
            return -2;
        }
        if(version < 0x10000u || version > 0x10004u) {
            return -3;
        }

        if(version > 0x10000u) {
            uint32_t submeshCount;
            if(!file.read(submeshCount)) {
                return -10;
            }
            if(!file.read(submeshes, submeshCount)) {
                return -10;
            }
        }

        struct {
            struct {
                uint32_t numIndices;
                uint32_t numVertices;
            } old;
            uint32_t vertexSize = sizeof(VertexBasic);
            uint32_t vertexType = 0;
        } geometry = {};

        if(version > 0x10003u) {
            if(!file.read(flags)) {
                return -20;
            }
            if(!file.read(geometry)) {
                return -21;
            }
            if(!file.read(boundingBox)) {
                return  -22;
            }
            if(!file.read(boundingSphere)) {
                return -23;
            }
        } else {
            flags = 0;
            if(!file.read(geometry.old)) {
                return  -30;
            }
            boundingBox = {};
            boundingSphere = {};
        }

        if(!file.read(indexData, geometry.old.numIndices)) {
            return -40;
        }

        if(geometry.vertexType == 0) {
            if(geometry.vertexSize != sizeof(VertexBasic)) {
                return -50;
            }
            auto& vertex = vertexData.emplace<std::vector<VertexBasic>>();
            if(!file.read(vertex, geometry.old.numVertices)) {
                return -51;
            }
        } else if(geometry.vertexType == 1) {
            if(geometry.vertexSize != sizeof(VertexWithColor)) {
                return -60;
            }
            auto& vertex = vertexData.emplace<std::vector<VertexWithColor>>();
            if(!file.read(vertex, geometry.old.numVertices)) {
                return -61;
            }
        } else {
            return -70;
        }

        if(version > 0x10001u) {
            if(!file.read(pivotPoint)) {
                return -80;
            }
        } else {
            pivotPoint = {};
        }
        return 0;
    }
};



#endif // RITOSKN_H
