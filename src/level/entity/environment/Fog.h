#pragma once

#include "../MovingEntity.h"

class __attribute__ ((packed)) Fog : public MovingEntity {

private:
    int16_t length;
    int8_t speed;

public:

    static Fog *spawn(uint8_t *args);

    void update();

    void draw();

    int16_t getSize();

};
