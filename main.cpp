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
    FILE* f = nullptr;
    fopen_s(&f, "room.nvr", "rb");
    File file(f);
    RitoNVR rito{};
    rito.load(file);

    cout << "Hello World!" << endl;

    return 0;
}
