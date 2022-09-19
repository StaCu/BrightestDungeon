#ifdef PLATFORM_ENTRY_POINT
/*
 * The PLATFORM_ENTRY_POINT is used by PlatformIO.
 * Arduino IDE uses the .ino file in the ../ directory.
 * However, Arduino IDE can't handle a second main function in the src/
 * directory.
 * This macro allows building with all both tool flows.
 */

#include <Platform.h>
#include "Dungeon.h"

ENTRY_POINT(
    Dungeon::setup();
    Dungeon::run();
)

#endif
