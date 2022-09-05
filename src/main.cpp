#ifdef NORMAL_ENTRY_POINT
/*
 * The NORMAL_ENTRY_POINT is used by PlatformIO.
 * Arduino IDE uses the .ino file in the ../ directory.
 * However, Arduino IDE can't handle a second main function in the src/
 * directory.
 * This macro allows building with all both tool flows.
 */

#include "Dungeon.h"

int main(int argc, const char **argv) {
    Dungeon::setup(argc, argv);
    Dungeon::run();
    return 0;
}

#endif
