#include "Worldgen.h"

#include <iostream>
#include <fstream>
#include <direct.h>

/* Currently windows only */

void WorldGen::test() {
    std::cout << "Called worldgen::test()\n";
    std::ofstream f("src/saves/test.txt");
    

    _mkdir("test");

    f << "Hello!\n";

    f.close();
}