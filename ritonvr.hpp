#ifndef RITONVR_HPP
#define RITONVR_HPP
#include "file.hpp"
#include "types.hpp"
#include "ritomath.hpp"
#include <variant>
#include <algorithm>

struct RitoNVR {
    struct Material {
        enum class Type : uint32_t {
            Default = 0,
            Decal = 1,
            WallOfGrass = 2,
            FourBlend = 3,
            AntiBrush = 4,
        };

        enum Flags : uint32_t {
            None,
            Ground = 0x01,
            NoShadow = 0x02,
            VertAlpha = 0x04,
            LightMapped = 0x08,
            DualVtxColor = 0x10,
            Background = 0x20,
            BkWithFOG = 0x40,
        };

        struct Channel {
            ColorF color = {};
            std::array<char, 260> textureName = {};
            Mtx44 transformMtx44 = Mtx44Zero;
        };

        struct Old {
            std::array<char, 260> name;
            Type type;
            ColorF diffuseColor;
            std::array<char, 260> diffuseTextureName;
            ColorF emissiveColor;
            std::array<char, 260> emissiveTextureName;
        };

        std::array<char, 260> name = {};
        Type type = {};
        Flags flags = {};
        std::array<Channel, 8> channels = {};
        Material() = default;
        Material(Material const&) noexcept = default;
        Material(Material&&) noexcept = default;
        Material& operator=(Material const&) = default;
        Material& operator=(Material&&) noexcept = default;
        Material(Old const& old) noexcept
            : name (old.name),
              type (old.type),
              flags (),
              channels({
                Channel { old.diffuseColor, old.diffuseTextureName, Mtx44Zero },
                Channel { old.emissiveColor, old.emissiveTextureName, Mtx44Zero }
              })
        {}
    };

    struct DrawIndexPrimitive {
        uint32_t vertexBuffer;
        uint32_t firstVertex;
        uint32_t vertexCount;
        uint32_t indexBuffer;
        uint32_t firstIndex;
        uint32_t indexCount;
    };

    struct Mesh {
        struct Old {
            int32_t qualityLevel;
            Sphere boundingSphere;
            Box3D boundingBox;
            uint32_t material;
            std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
        };

        int32_t qualityLevel;
        uint32_t flags;
        Sphere boundingSphere;
        Box3D boundingBox;
        uint32_t material;
        std::array<DrawIndexPrimitive, 2> drawIndexPrimitives;
        Mesh() = default;
        Mesh(Mesh const&) = default;
        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh const&) = default;
        Mesh& operator=(Mesh&&) noexcept = default;
        Mesh(Old const& other) noexcept
            : qualityLevel(other.qualityLevel),
              flags(),
              boundingSphere(other.boundingSphere),
              boundingBox(other.boundingBox),
              material(other.material),
              drawIndexPrimitives(other.drawIndexPrimitives)
        {}
    };

    struct Node {
        Box3D boundingBox;
        int32_t firstMesh;
        int32_t meshCount;
        int32_t firstChildNode;
        int32_t childNodeCount;
    };

    std::vector<Material> materials;
    std::vector<std::vector<uint8_t>> vertexBuffers;
    std::vector<std::vector<uint32_t>> indexBuffers;
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

        if(header.version == 0x10008) {
            std::vector<Material::Old> old{};
            if(!file.read(old, header.materialCount)) {
                return -4;
            }
            materials = { old.begin(), old.end() };
        } else {
            if(!file.read(materials, header.materialCount)) {
                return -5;
            }
        }

        for(uint32_t i = 0; i < header.vertexBufferCount; i++) {
            uint32_t size = 0;
            if(!file.read(size)) {
                return  -6;
            }
            auto& buffer = vertexBuffers.emplace_back();
            if(!file.read(buffer, size)) {
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
                std::vector<uint16_t> buffer{};
                if(!file.read(buffer, size / sizeof(uint16_t))) {
                    return -10;
                }
                indexBuffers.emplace_back(buffer.begin(), buffer.end());
            } else if(format == 0x66) {
                auto& buffer = indexBuffers.emplace_back();
                if(!file.read(buffer, size / sizeof(uint32_t))) {
                    return -11;
                }
            } else {
                return -12;
            }
        }

        if(header.version == 0x10008) {
            std::vector<Mesh::Old> old{};
            if(!file.read(old, header.meshCount)) {
                return -13;
            }
            meshes = { old.begin(), old.end() };
        } else {
            if(!file.read(meshes, header.meshCount)) {
                return -14;
            }
        }
        if(!file.read(nodes, header.nodeCount)) {
            return -15;
        }
        return 0;
    }

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

    template<typename T>
    struct BufferRange {
      T const* const startp;
      T const* const endp;
      BufferRange() = default;
      BufferRange(std::vector<uint8_t> const& vec, uint32_t first, uint32_t count) noexcept
        : startp(reinterpret_cast<T const*>(vec.data()) + first),
          endp(reinterpret_cast<T const*>(vec.data()) + first + count)
      { }
    };

    using VertexBufferRange = std::variant<
        BufferRange<VertexDefault>,
        BufferRange<VertexPosition>,
        BufferRange<VertexUV2>,
        // BufferRange<VertexUV3>,
        BufferRange<VertexColor2>
        // BufferRange<VertexDecal>,
        // BufferRange<VertexPositionUv1>
    >;

    VertexBufferRange getVertexBufferRange(Mesh const& mesh,
                                           size_t drawPrimitiveIndex) const& noexcept {
        auto const& p = mesh.drawIndexPrimitives[drawPrimitiveIndex];
        auto const& buffer = vertexBuffers[p.vertexBuffer];
        if(drawPrimitiveIndex == 1) {
            return BufferRange<VertexPosition> { buffer, p.firstVertex, p.vertexCount };
        }
        auto const& mat = materials[mesh.material];
        switch (mat.type) {
            case Material::Type::Default:
                if(!(mat.flags & Material::Flags::DualVtxColor)) {
                    return BufferRange<VertexDefault> { buffer, p.firstVertex, p.vertexCount };
                }
                return BufferRange<VertexColor2> { buffer, p.firstVertex, p.vertexCount };
            case Material::Type::Decal:
                return BufferRange<VertexDefault> { buffer, p.firstVertex, p.vertexCount };
            case Material::Type::WallOfGrass:
                return BufferRange<VertexDefault> { buffer, p.firstVertex, p.vertexCount };
            case Material::Type::FourBlend:
                return BufferRange<VertexUV2> { buffer, p.firstVertex, p.vertexCount };
            case Material::Type::AntiBrush:
                return BufferRange<VertexDefault> { buffer, p.firstVertex, p.vertexCount };
        }
        return BufferRange<VertexDefault> { buffer, p.firstVertex, p.vertexCount };
    }

    auto dumpVtxs() const& noexcept {
        std::vector<VertexBufferRange> vtxs {};
        for(auto const& node: nodes) {
            if(!node.meshCount) {
                continue;
            }
            for(auto i = node.firstMesh; i < (node.firstMesh + node.meshCount); i++) {
                vtxs.push_back(getVertexBufferRange(meshes[i], 0));
                vtxs.push_back(getVertexBufferRange(meshes[i], 1));
            }
        }
        return vtxs;
    }
};

#endif // RITONVR_HPP
