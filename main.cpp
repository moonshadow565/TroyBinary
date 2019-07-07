#include <iostream>
#include "inibin.h"
#include "particle/system.h"
#include "ritoskn.hpp"
#include "ritoskl.hpp"
#include "ritoanm.hpp"
#include "ritonvr.hpp"

using namespace std;

int main()
{
    /*
    Ini ini{};
    ini.from_file("GemGlow.troybin");

    RitoParticle::System system{};
    system.load(ini);

    */
    if(auto file = File::readb("room.nvr"); file) {
        RitoNVR nvr{};
        nvr.load(*file);
        auto vtx = RitoNVRTest::dump_vtxs(nvr);
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
