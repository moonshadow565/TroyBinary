#include <iostream>
#include "inibin.h"
#include "particle/system.h"

using namespace std;

int main()
{
    Ini ini{};
    ini.from_file("GemGlow.troybin");

    RitoParticle::System system{};
    system.load(ini);

    cout << "Hello World!" << endl;

    return 0;
}
