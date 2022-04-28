#pragma once

#include <stdint.h>

class VictoryAnimation {

private:

    static uint8_t count;

public:

    static void start();
    
    static void update();

    static void draw();

    static bool isActive();

};
