#include <iostream>
#include "inibin.h"
#include "particle/system.h"
#include "ritoskn.hpp"
#include "ritoskl.hpp"
#include "ritoanm.hpp"
#include "ritonvr.hpp"

using namespace std;

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

RitoNVR::VertexType load_nvr_material_vtx_type(RitoNVR const& nvr, uint32_t index) {
    RitoNVR::MaterialType mtype;
    RitoNVR::MaterialFlags mflags;
    auto const& mat = nvr.materials[index];
    mtype = mat.type;
    mflags = mat.flags;
    return RitoNVR::VertexTypeFromMaterial(mtype, mflags);
}

template<typename T>
void load_nvr_vtx(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                  uint32_t vertexBuffer, uint32_t firstVertex, uint32_t vertexCount) {
    auto const& buffer = nvr.vertexBuffers[vertexBuffer];
    auto start = reinterpret_cast<T const*>(buffer.data.data()) + firstVertex;
    auto end = start + vertexCount;
    vtx.insert(vtx.end(), start, end);
}

void load_nvr_mesh(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                   RitoNVR::Mesh const& mesh) {
    for(auto const& p: mesh.drawIndexPrimitives) {
        if(!p.indexCount) {
            continue;
        }
        load_nvr_vtx<RitoNVR::VertexPosition>(nvr, vtx,
                                              p.vertexBuffer, p.firstVertex, p.vertexCount);
    }
    /*
    switch(load_nvr_material_vtx_type(nvr, mesh.m_Material)) {
    case RitoNVR::VertexType::Default:
        return load_nvr_primitives<RitoNVR::VertexDefault>(nvr, vtx, mesh.drawIndexPrimitives);
    case RitoNVR::VertexType::Uv2:
        return load_nvr_primitives<RitoNVR::VertexUV2>(nvr, vtx, mesh.drawIndexPrimitives);
    case RitoNVR::VertexType::Color2:
        return load_nvr_primitives<RitoNVR::VertexColor2>(nvr, vtx, mesh.drawIndexPrimitives);
    default:
        return;
    }
    */
}

void load_nvr_node(RitoNVR const& nvr, std::vector<VertexTotal>& vtx,
                   RitoNVR::Node const& node){
    if(node.meshCount) {
        for(auto i = node.firstMesh; i < (node.firstMesh + node.meshCount); i++) {
            load_nvr_mesh(nvr, vtx, nvr.meshes[static_cast<size_t>(i)]);
        }
    }
}

int main()
{
    /*
    Ini ini{};
    ini.from_file("GemGlow.troybin");

    RitoParticle::System system{};
    system.load(ini);

    */
    if(auto file = File::readb("room.nvr"); file){

        RitoNVR nvr{};
        nvr.load(*file);
        std::vector<VertexTotal> vtx{};
        for(auto const& node: nvr.nodes) {
            load_nvr_node(nvr, vtx, node);
        }
        for(auto const& v: vtx) {
            printf("%f %f %f\n",
                   v.position.x, v.position.y, v.position.z);
        }
    }
    if(auto file = File::readb("Akali_Attack1.anm"); file) {
        RitoANM anm{};
        anm.load(*file);
    }
    if(auto file = File::readb("Akali_nurse.skl"); file) {
        RitoSKL skl{};
        skl.load(*file);
    }
    if(auto file = File::readb("Akali_nurse.skn"); file) {
        RitoSKN skn{};
        skn.load(*file);
    }

    return 0;
}
