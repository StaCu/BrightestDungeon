#pragma once

#include "Entity.h"
#include "LongEntity.h"
#include "MovingEntity.h"
#include "hero/Hero.h"
#include "hero/Shot.h"
#include "hero/Sword.h"
#include "monster/Boss.h"
#include "monster/Monster.h"
#include "monster/Spawner.h"
#include "monster/Worm.h"
#include "environment/Button.h"
#include "environment/Door.h"
#include "environment/Fog.h"
#include "environment/ForceField.h"
#include "environment/Item.h"
#include "environment/Lava.h"
#include "environment/Mine.h"
#include "environment/Portal.h"
#include "environment/Switch.h"
#include "environment/Wall.h"
#include "environment/Water.h"

/**
 * Union which can hold one type of entity at once.
 * This implementation here is technically undefined behaviour,
 * because .type and .entity are used to access the same fields 
 * present in the individual entities.
 * However, it works...
 * 
 * The different methods here are a type of "virtual" method.
 * Internally, they select the method of the corresponding type.
 */
union __attribute__ ((packed)) EntityUnion {
public:
    uint8_t type;
    Entity entity;
    MovingEntity moving_entity;
    LongEntity long_entity;

    Hero hero;
    Shot shot;
    Sword sword;

    Boss boss;
    Monster monster;
    Spawner spawner;
    Worm worm;

    Button button;
    Door door;
    Fog fog;
    ForceField forcefield;
    Item item;
    Lava lava;
    Mine mine;
    Portal portal;
    Switch sw;
    Wall wall;
    Water water;

    EntityUnion();

    bool isActive() const;

    void update();

    void draw();

    static void collision(EntityUnion *a, EntityUnion *b);

    static int16_t getCollisionDepth(int16_t a_pos, int16_t a_size, int16_t a_old_pos, int16_t b_pos, int16_t b_size, int16_t b_old_pos);

    int16_t getPosition();
    int16_t getOldPosition();
    int16_t getSize();

};
