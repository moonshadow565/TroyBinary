#ifndef RITONVR_HPP
#define RITONVR_HPP
#include "file.hpp"
#include "types.hpp"

struct RitoNVR {
    enum class MaterialType : uint32_t {
        Default = 0,
        Decal = 1,
        WallOfGrass = 2,
        FourBlend = 3,
        AntiBrush = 4,
    };

    enum MaterialFlags : uint32_t {
        None,
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
        ColorF diffuseColor;
        char diffuseTextureName[260];
        ColorF emissiveColor;
        char emissiveTextureName[260];
    };

    struct Material {
        struct Channel {
            ColorF color;
            char textureName[260];
            Mtx44 transformMtx44;
        };
        char name[260];
        MaterialType type;
        MaterialFlags flags;
        std::array<Channel, 8> channels;
    };

    enum class VertexType : uint32_t {
        Default = 0,
        Position = 1,
        Uv2 = 2,
        Uv3 = 3,
        Color2 = 4,
        Decal = 5,
        PosUv1 = 6,
    };
    struct VertexDefault {
        Vec3 position;
        Vec3 normal;
        Vec2 textCoord0;
        ColorB color0;
    };
    struct VertexPosition {
        Vec3 position;
    };
    struct VertexUV2 {
        Vec3 position;
        Vec3 normal;
        Vec2 textCoord0;
        Vec2 textCoord1;
        ColorB color0;
    };
    struct VertexUV3 {
        Vec3 position;
        Vec3 normal;
        Vec2 textCoord0;
        Vec2 textCoord1;
        Vec2 textCoord2;
        ColorB color0;
    };
    struct VertexColor2 {
        Vec3 position;
        Vec3 normal;
        Vec2 textCoord0;
        ColorB color0;
        ColorB color1;
    };
    struct VertexDecal {
        Vec3 position;
    };
    struct VertexPositionUv1 {
        Vec3 position;
        Vec2 textCoord0;
    };

    inline static constexpr VertexType VertexTypeFromMaterial(MaterialType type,
                                                              MaterialFlags flags) noexcept {
        switch (type) {
            case MaterialType::Default:
                if(!(flags & MaterialFlags::DualVtxColor)) {
                    return VertexType::Default;
                }
                return VertexType::Color2;
            case MaterialType::Decal:
                return VertexType::Default;
            case MaterialType::WallOfGrass:
                return VertexType::Default;
            case MaterialType::FourBlend:
                return VertexType::Uv2;
            case MaterialType::AntiBrush:
                return VertexType::Default;
        }
        return VertexType::Default;
    }

    struct VertexBuffer {
        // if something called dipIndex is set to 1 then Type::Position is used instead
        std::vector<uint8_t> data;
    };

    struct Sphere {
        Vec3 centerPoint;
        float radius;
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
        int32_t qualityLevel;
        Sphere boundingSphere;
        Box3D boundingBox;
        uint32_t material;
        std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
    };
    struct Mesh {
        int32_t qualityLevel;
        uint32_t flags;
        Sphere boundingSphere;
        Box3D boundingBox;
        uint32_t material;
        std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
    };

    // some of those can contain -1 as probably as optional value
    struct Node {
        Box3D boundingBox;
        int32_t firstMesh;
        int32_t meshCount;
        int32_t firstChildNode;
        int32_t childNodeCount;
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

// Test NVR
namespace RitoNVRTest{
    struct VertexTotal {
        Vec3 position;
        Vec3 normal;
        Vec2 textCoord0;
        Vec2 textCoord1;
        ColorB color0;
        ColorB color1;

        VertexTotal(RitoNVR::VertexPosition other) noexcept
            : position(other.position),
              normal({}),
              textCoord0({}),
              textCoord1({}),
              color0({}),
              color1({})
        {}

        VertexTotal(RitoNVR::VertexDefault other) noexcept
            : position(other.position),
              normal(other.normal),
              textCoord0(other.textCoord0),
              textCoord1({}),
              color0(other.color0),
              color1({})
        {}

        VertexTotal(RitoNVR::VertexUV2 other) noexcept
            : position(other.position),
              normal(other.normal),
              textCoord0(other.textCoord0),
              textCoord1(other.textCoord1),
              color0(other.color0),
              color1({})
        {}

        VertexTotal(RitoNVR::VertexColor2 other) noexcept
            : position(other.position),
              normal(other.normal),
              textCoord0(other.textCoord0),
              textCoord1({}),
              color0(other.color0),
              color1(other.color1)
        {}
    };

    template<typename T>
    inline void load_nvr_vtx(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                      uint32_t vertexBuffer, uint32_t firstVertex, uint32_t vertexCount) {
        auto const& buffer = nvr.vertexBuffers[vertexBuffer];
        auto start = reinterpret_cast<T const*>(buffer.data.data()) + firstVertex;
        auto end = start + vertexCount;
        vtx.insert(vtx.end(), start, end);
    }

    inline void load_nvr_vtx_t(RitoNVR const& nvr, std::vector<VertexTotal>& vtx, RitoNVR::VertexType t,
                      uint32_t vertexBuffer, uint32_t firstVertex, uint32_t vertexCount) {
        switch(t) {
        case RitoNVR::VertexType::Default:
            return load_nvr_vtx<RitoNVR::VertexDefault>(nvr, vtx,
                                                        vertexBuffer, firstVertex, vertexCount);
        case RitoNVR::VertexType::Position:
            return load_nvr_vtx<RitoNVR::VertexPosition>(nvr, vtx,
                                                         vertexBuffer, firstVertex, vertexCount);
        case RitoNVR::VertexType::Uv2:
            return load_nvr_vtx<RitoNVR::VertexUV2>(nvr, vtx,
                                                    vertexBuffer, firstVertex, vertexCount);
        case RitoNVR::VertexType::Color2:
            return load_nvr_vtx<RitoNVR::VertexColor2>(nvr, vtx,
                                                       vertexBuffer, firstVertex, vertexCount);
        default:
            return;
        }
    }

    inline void load_nvr_mesh(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                       RitoNVR::Mesh const& mesh) {
        auto const& mat = nvr.materials[mesh.material];
        auto const vt = RitoNVR::VertexTypeFromMaterial(mat.type, mat.flags);
        auto p0 = mesh.drawIndexPrimitives[0];
        load_nvr_vtx_t(nvr, vtx, vt, p0.vertexBuffer, p0.firstVertex, p0.vertexCount);
        auto p1 = mesh.drawIndexPrimitives[1];
        load_nvr_vtx<RitoNVR::VertexPosition>(nvr, vtx, p1.vertexBuffer, p1.firstVertex, p1.vertexCount);
    }

    inline void load_nvr_node(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                       RitoNVR::Node const& node){
        // Actual loading loads the nodes either as parents or childrens
        // So only meshes from meshes that aren't perents but only children have their nodes loaded ??
        if(node.meshCount) {
            for(auto i = node.firstMesh; i < (node.firstMesh + node.meshCount); i++) {
                load_nvr_mesh(nvr, vtx, nvr.meshes[static_cast<size_t>(i)]);
            }
        }
    }

    inline auto dump_vtxs(RitoNVR const& nvr) noexcept {
        std::vector<VertexTotal> vtx{};
        for(auto const& node: nvr.nodes) {
            load_nvr_node(nvr, vtx, node);
        }
        return vtx;
    }
}


#endif // RITONVR_HPP
