#ifndef RITONVR_HPP
#define RITONVR_HPP
#include "file.hpp"
#include "types.hpp"

struct RitoNVR {
    enum MaterialType : uint32_t {
        Default = 0,
        Decal = 1,
        WallOfGrass = 2,
        FourBlend = 3,
        AntiBrush = 4,
    };

    enum MaterialFlags : uint32_t {
        Ground = 0x01,
        NoShadow = 0x02,
        VertAlpha = 0x04,
        LightMapped = 0x08,
        DualVtxColor = 0x10,
        Background = 0x20,
        BkWithFOG = 0x40,
    };

    struct MaterialOld {
        char name[260];
        MaterialType type;
        std::array<float, 4> diffuseColor;
        char diffuseTextureName[260];
        std::array<float, 4> emissiveColor;
        char emissiveTextureName[260];
    };

    struct Material {
        struct Channel {
            std::array<float, 4> color;
            char textureName[260];
            std::array<std::array<float, 4>, 4> transformMtx44;
        };
        char name[260];
        MaterialType type;
        MaterialFlags flags;
        std::array<Channel, 8> channels;
    };

    struct VertexBuffer {
        enum VertexType : uint32_t {
            Default = 0,
            Position = 1,
            Uv2 = 2,
            Uv3 = 3,
            Color2 = 4,
            Decal = 5,
            PosUv1 = 6,
        };

        struct VertexDefault {
            std::array<float, 3> position;
            std::array<float, 3> normal;
            std::array<float, 2> textCoord0;
            std::array<uint8_t, 4> color0;
        };
        struct VertexPosition {
            std::array<float, 3> position;
        };
        struct VertexUV2 {
            std::array<float, 3> position;
            std::array<float, 3> normal;
            std::array<float, 2> textCoord0;
            std::array<float, 2> textCoord1;
            std::array<uint8_t, 4> color0;
        };
        struct VertexUV3 {
            std::array<float, 3> position;
            std::array<float, 3> normal;
            std::array<float, 2> textCoord0;
            std::array<float, 2> textCoord1;
            std::array<float, 2> textCoord2;
            std::array<uint8_t, 4> color0;
        };
        struct VertexColor2 {
            std::array<float, 3> position;
            std::array<float, 3> normal;
            std::array<float, 2> textCoord0;
            std::array<uint8_t, 4> color0;
            std::array<uint8_t, 4> color1;
        };
        struct VertexDecal {
            std::array<float, 3> position;
        };
        struct VertexPositionUv1 {
            std::array<float, 3> position;
            std::array<float, 2> textCoord0;
        };

        inline constexpr static size_t VertexSizes[] = {
            sizeof (VertexDefault),
            sizeof (VertexPosition),
            sizeof (VertexUV2),
            sizeof (VertexUV3),
            sizeof (VertexColor2),
            sizeof (VertexDecal),
            sizeof (VertexPositionUv1),
        };

        // if something called dipIndex is set to 1 then Type::Position is used instead
        inline static constexpr VertexType typeFromMaterial(MaterialType type,
                                                            MaterialFlags flags) noexcept {
            switch (type) {
                case MaterialType::Default:
                    return flags & MaterialFlags::DualVtxColor ?
                                VertexType::Color2 : VertexType::Default;
                case MaterialType::Decal:
                    return VertexType::Default;
                case MaterialType::WallOfGrass:
                    return VertexType::Default;
                case MaterialType::FourBlend:
                    return VertexType::Uv2;
                case MaterialType::AntiBrush:
                    return VertexType::Default;
            }
        }

        std::vector<uint8_t> data;
    };

    struct Sphere {
        std::array<float, 3> centerPoint;
        float radius;
    };
    struct Box3 {
        std::array<float, 3> minPoint;
        std::array<float, 3> maxPoint;
    };

    struct DrawIndexPrimitive {
        uint32_t vertexBuffer;
        uint32_t firstVertex;
        uint32_t vertexCount;
        uint32_t indexBuffer;
        uint32_t firstIndex;
        uint32_t indexCount;
    };
    struct MeshOld {
        int32_t m_QualityLevel;
        Sphere boundingSphere;
        Box3 boundingBox;
        uint32_t m_Material;
        std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
    };
    struct Mesh {
        int32_t m_QualityLevel;
        uint32_t m_Flags;
        Sphere boundingSphere;
        Box3 boundingBox;
        uint32_t m_Material;
        std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
    };

    // some of those can contain -1 as probably as optional value
    struct Node {
        Box3 boundingBox;
        uint32_t firstMesh;
        uint32_t meshCount;
        uint32_t firstChildNode;
        uint32_t childNodeCount;
    };

    std::vector<MaterialOld> materialsOld;
    std::vector<Material> materials;
    std::vector<VertexBuffer> vertexBuffers;
    std::vector<std::vector<uint32_t>> indexBuffers;
    std::vector<MeshOld> meshesOld;
    std::vector<Mesh> meshes;
    std::vector<Node> nodes;

    int load(File const& file) noexcept {
        struct Header {
            std::array<char, 4> magic;
            uint32_t version;
            uint32_t materialCount;
            uint32_t vertexBufferCount;
            uint32_t indexBufferCount;
            uint32_t meshCount;
            uint32_t nodeCount;
        } header;

        if(!file.read(header)) {
            return -1;
        }
        if(header.magic != std::array{'N', 'V', 'R', '\0'}) {
            return -2;
        }
        if(header.version != 0x10008 && header.version != 0x10009) {
            return -3;
        }

        if(header.version == 0x10008 && !file.read(materialsOld, header.materialCount)) {
            return -4;
        }
        if(header.version == 0x10009 && !file.read(materials, header.materialCount)) {
            return -5;
        }

        for(uint32_t i = 0; i < header.vertexBufferCount; i++) {
            uint32_t size = 0;
            if(!file.read(size)) {
                return  -6;
            }
            if(auto& buffer = vertexBuffers.emplace_back(); !file.read(buffer.data, size)) {
                return  -7;
            }
        }

        for(uint32_t i = 0; i < header.indexBufferCount; i++) {
            uint32_t size;
            uint32_t format;
            if(!file.read(size)) {
                return -9;
            }
            if(!file.read(format)) {
                return -8;
            }
            if (format == 0x65) {
                std::vector<uint16_t> tmpvalue{};
                if(!file.read(tmpvalue, size / sizeof(uint16_t))) {
                    return -10;
                }
                indexBuffers.emplace_back(tmpvalue.begin(), tmpvalue.end());
            } else if(format == 0x66) {
                if(auto& value = indexBuffers.emplace_back();
                        !file.read(value, size / sizeof(uint32_t))) {
                    return -11;
                }
            } else {
                return -12;
            }
        }

        if(header.version == 0x10008 && !file.read(meshesOld, header.meshCount)) {
            return -13;
        }
        if(header.version == 0x10009 && !file.read(meshes, header.meshCount)) {
            return -14;
        }

        if(!file.read(nodes, header.nodeCount)) {
            return -15;
        }
        return 0;
    }
};

#endif // RITONVR_HPP
