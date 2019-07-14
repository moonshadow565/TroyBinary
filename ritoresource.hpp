#ifndef RITORESOURCE_HPP
#define RITORESOURCE_HPP
#include <cinttypes>
#include <vector>
#include <array>
#include "types.hpp"
#include "file.hpp"

namespace RitoResource {
    struct Animation {
        struct Channel {
            struct Tick {
                uint32_t boneHash;
                Vec3 position;
                Vec3 scale;
                Quat quat;
            };
            std::vector<Tick> ticks;
        };
        std::vector<Channel> channels;

        int load_v4(File const& file) {
            struct TickSaved {
                uint32_t boneHash;
                uint16_t posIndx;
                uint16_t scaleIndx;
                uint16_t quatIndx;
                uint16_t pad;
            };
            struct {
                uint32_t resourceSize;
                uint32_t formatToken;
                uint32_t version;
                uint32_t flags;
                uint32_t numChannels;
                uint32_t numTicks;
                float tickDuration;
                uint32_t channelsOffset;
                uint32_t assetNameOffset;
                uint32_t timeOffset;
                uint32_t vectorPaletteOffset;
                uint32_t quatPaletteOffset;
                uint32_t tickDataOffset;
                uint32_t extBuffer[3];
            } v4 {};
            std::vector<uint8_t> data{};
            file.read(v4);
            data.resize(v4.resourceSize - sizeof(v4));
            file.read(data.data(), data.size());

            auto const vecStartAddr = data.data() + v4.vectorPaletteOffset - sizeof(v4);
            auto const vecStart = reinterpret_cast<Vec3 const*>(vecStartAddr);

            auto const quatStartAddr = data.data() + v4.quatPaletteOffset - sizeof(v4);
            auto const quatStart = reinterpret_cast<Quat const*>(quatStartAddr);

            auto const tickStartAddr = data.data() + v4.tickDataOffset - sizeof(v4);
            auto const tickStart = reinterpret_cast<TickSaved const*>(tickStartAddr);

            for(uint32_t c = 0; c < v4.numChannels; c++) {
                auto& channel = channels.emplace_back();
                auto& ticks = channel.ticks;
                ticks.reserve(v4.numTicks);
                for(uint32_t t = 0; t < v4.numTicks; t++) {
                    auto const& savedTick = tickStart[c * v4.numTicks + t];
                    ticks.push_back(Channel::Tick {
                                        savedTick.boneHash,
                                        vecStart[savedTick.posIndx],
                                        vecStart[savedTick.scaleIndx],
                                        quatStart[savedTick.quatIndx]
                                    });
                }
            }

            return 0;
        }

        int load(File const& file) {
            struct {
                std::array<char, 8> magic;
                uint32_t version;
            } header;
            auto const fileBegin = file.tell();
            if(!file.read(header)) {
                return -1;
            }
            return load_v4(file);

            return 0;
        }
    };

    struct Skeleton {
        struct Form {
            Vec3 position;
            Vec3 scale;
            Quat orientation;
        };
        struct Joint {
            uint16_t flags;
            uint16_t jointIndx;
            int16_t parentIndx;
            uint16_t pad;
            uint32_t nameHash;
            float radius;
            Form parentOffset;
            Form invRootOffset;
            std::string name;
        };
        std::string name;
        std::string assetName;
        std::vector<uint32_t> shaderBones;
        std::vector<Joint> joints;
        int load(File const& file) {
            struct V0{
                uint32_t resourceSize;
                uint32_t formatToken;
                uint32_t version;
                uint16_t flags;
                uint16_t numJoints;
                uint32_t numShaderBones;
                uint32_t jointArrOffset;
                uint32_t jointHashIdxOffset;
                uint32_t shaderJointArrOffset;
                uint32_t nameOffset;
                uint32_t assetNameOffset;
                uint32_t jointNamesOffset;
                uint32_t extBuffer[5];
            };
            struct JointHashSaved {
                uint16_t jointIndex;
                uint16_t pad;
                uint32_t jointHash;
            };
            struct JointSaved {
                  uint16_t flags;
                  uint16_t jointNdx;
                  int16_t parentIndx;
                  uint16_t pad;
                  uint32_t nameHash;
                  float radius;
                  Form parentOffset;
                  Form invRootOffset;
                  uint32_t name;
            };
            uint32_t resourceSize;
            file.read(resourceSize);
            file.seek_cur(-sizeof(resourceSize));

            std::vector<uint8_t> data;
            data.resize(resourceSize);
            file.read(data.data(), data.size());

            auto const& v0 = *reinterpret_cast<V0 const*>(data.data());
            auto const jointArrAddr = data.data() + v0.jointArrOffset;
            auto const joinHashIdxAddr = data.data() + v0.jointHashIdxOffset;
            auto const shaderJointArrAddr = data.data() + v0.shaderJointArrOffset;
            auto const nameAddr = data.data() + v0.nameOffset;
            auto const assetNameAddr = data.data() + v0.assetNameOffset;
            auto const jointNamesAddr =  data.data() + v0.jointNamesOffset;

            if(v0.nameOffset != 0u && v0.nameOffset != 0xFFFFFFFFu) {
                name = reinterpret_cast<char const*>(nameAddr);
            }
            if(v0.assetNameOffset != 0u && v0.assetNameOffset != 0xFFFFFFFFu) {
                assetName = reinterpret_cast<char const*>(assetNameAddr);
            }
            auto const jointSavedArr = reinterpret_cast<JointSaved const*>(jointArrAddr);
            // auto const jointHashIdxArr = reinterpret_cast<JointHashSaved const*>(joinHashIdxAddr);
            auto const shaderJointArr = reinterpret_cast<uint32_t const*>(shaderJointArrAddr);
            // auto const jointNamesArr = reinterpret_cast<char const*>(jointNamesAddr);

            joints.reserve(v0.numJoints);
            shaderBones =  { shaderJointArr , shaderJointArr + v0.numShaderBones };

            for(uint32_t i = 0; i < v0.numJoints; i++) {
                auto const& saved = jointSavedArr[i];
                auto const name = reinterpret_cast<char const*>(&saved.name) + saved.name;
                joints.push_back(Joint {
                                     saved.flags,
                                     saved.jointNdx,
                                     saved.parentIndx,
                                     saved.pad,
                                     saved.nameHash,
                                     saved.radius,
                                     saved.parentOffset,
                                     saved.invRootOffset,
                                     name
                                 });
            }

            return 0;
        }
    };
}



#endif // RITORESOURCE_HPP
