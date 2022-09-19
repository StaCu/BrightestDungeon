/**
 * In order to build BrightestDungeon with ArduinoIDE,
 * copy the libraries from platform/<platform>/lib/ and
 * the required common libs from platform/lib/ to the
 * ArduinoIDE library folder.
 * The concrete list of required libs can be found in
 * platform/<platform>/Platform.md
 * Afterwards compile and upload should work normally.
 */

#include "src/Dungeon.h"

void setup() {
    Dungeon::setup();
    Dungeon::run();
}

void loop() {
    
}
