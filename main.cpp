#include <iostream>
#include <filesystem>
#include "inibin.h"
#include "particle/system.h"
#include "ritoskn.hpp"
#include "ritoskl.hpp"
#include "ritoanm.hpp"
#include "ritonvr.hpp"
#include "ritoresource.hpp"


using namespace std;

int main()
{
    /*
    Ini ini{};
    ini.from_file("GemGlow.troybin");

    RitoParticle::System system{};
    system.load(ini);

    */
    if(auto file = File::readb("Anivia3/Animations/anivia_laugh.anm"); file) {
        RitoResource::Animation anm {};
        anm.load(*file);
    }
    return 0;
}
