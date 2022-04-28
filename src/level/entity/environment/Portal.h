#pragma once

#include "../Entity.h"

class __attribute__ ((packed)) Portal : public Entity {

private:
    /**
     * where the paired portal is located (i.e. where the player will exit, when entering this portal)
     */
    int16_t exit;
    /**
     * when inverted, entering and exiting look as follows:
     * enter              exit
     * <->blue              <->orange
     *    blue<->              orange<->
     * 
     * when not inverted, entering and exiting look as follows:
     * enter              exit
     * <->orange                 blue<->
     *    orange<->           <->blue
     */
    bool inverted;

    static Portal *spawn_one(uint8_t mask, int16_t pos0, int16_t pos1, bool inverted);

public:
    static Portal *spawn(uint8_t *args);
    static Portal *spawn(uint8_t mask, int16_t pos0, int16_t pos1);

    void update();

    void collision(Entity &entity, int16_t depth);

    void draw();

    int16_t getSize() const;

    int16_t getExit() const;

    bool isInverted() const;

};
