#pragma once

#include "../../../Settings.h"
#include "../Entity.h"

class Hero;

enum ItemType : uint8_t {
    SET_COLOR_0 = 0,
    SET_COLOR_1 = 1,
    SWAP_ATTACK_TYPE = 2,
    INVERT_CONTROL = 3,
    EXTRA_LIFE = 4,
};

class __attribute__ ((packed)) Item : public Entity {

private:
    ItemType type;

    /**
     * Animation of the extra_life item.
     * The code for generating this animation was larger than saving each frame in memory...
     */
    static const PROGMEM uint8_t led_item_life[];

public:

    static Item *spawn(uint8_t *args);

    static Item *spawn(uint8_t mask, int16_t pos, ItemType type);

    void update();

    void collision(Entity &entity, int16_t depth);

    void collision(Hero &hero, int16_t depth);

    void draw();

    int16_t getSize();

    ItemType getType();

};
