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
            channels.resize(v4.numChannels);

            auto const vecStartAddr = data.data() + v4.vectorPaletteOffset - sizeof(v4);
            auto const vecStart = reinterpret_cast<Vec3 const*>(vecStartAddr);

            auto const quatStartAddr = data.data() + v4.quatPaletteOffset - sizeof(v4);
            auto const quatStart = reinterpret_cast<Quat const*>(quatStartAddr);

            auto const tickStartAddr = data.data() + v4.tickDataOffset - sizeof(v4);
            auto const tickStart = reinterpret_cast<TickSaved const*>(tickStartAddr);


            for(auto& channel: channels) {
                auto& ticks = channel.ticks;
                ticks.reserve(v4.numTicks);
                for(uint32_t t = 0; t < v4.numTicks; t++) {
                    auto const& savedTick = tickStart[t];
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
}



#endif // RITORESOURCE_HPP
