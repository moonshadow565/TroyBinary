#ifndef RITOANM_HPP
#define RITOANM_HPP
#include "file.hpp"
#include "types.hpp"
#include "ritomath.hpp"

struct RitoANM {
    struct Frame {
        std::array<float, 4> quaternion;
        Vec3 point;
    };

    struct Track {
        std::array<char, 32> name;
        // flag 2 is transform from initial
        uint32_t flags;
        std::vector<Frame> frames;
    };

    uint32_t skeletonID;
    int32_t numTracks;
    int32_t numFrames;
    int32_t frameRate;
    std::vector<Track> tracks{};

    int read_v1(File const& file) noexcept {
        if(!file.read(skeletonID)) {
            return -3;
        }
        if(!file.read(numTracks)) {
            return -4;
        }
        if(!file.read(numFrames)) {
            return -5;
        }
        if(!file.read(frameRate)) {
            return -6;
        }
        for(int32_t t = 0; t < numTracks; t++) {
            auto& track = tracks.emplace_back();
            if(!file.read(track.name)) {
                return -7;
            }
            if(!file.read(track.flags)) {
                return -8;
            }
            if(!file.read(track.frames, static_cast<size_t>(numFrames))) {
                return -9;
            }
        }
        return 0;
    }

    int load(File const& file) noexcept {
        std::array<char, 8> magic;
        uint32_t version;
        if(!file.read(magic) || !file.read(version)) {
            return -1;
        }
        if(magic != std::array{'r', '3', 'd', '2', 'a', 'n', 'm', 'd'} || version != 3 ) {
            return -2;
        }
        return read_v1(file);
    }
};


#endif // RITOANM_HPP
