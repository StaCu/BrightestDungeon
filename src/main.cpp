#ifdef NORMAL_ENTRY_POINT
/*
 * The NORMAL_ENTRY_POINT is used by PlatformIO and CMake.
 * Arduino IDE uses the .ino file in the ../ directory.
 * However, Arduino IDE can't handle a second main function in the src/
 * directory.
 * This macro allows building with all three tool flows.
 */

#include "Settings.h"
#include "Dungeon.h"

int main(int argc, const char **argv) {
#ifdef SIMULATOR
    /*
     * The SIMULATOR macro appears in some parts of the code.
     * Some parts of the Embedded code is not available when compiling for a 
     * desktop computer (e.g. libs, io ports, rng) and get replaced by
     * using this macro
     */
    Simulator::init(argc, argv);
#endif
    Dungeon::setup();
    Dungeon::run();
    return 0;
}

#endif
