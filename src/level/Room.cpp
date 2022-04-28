#include "Room.h"

#include "../Settings.h"
#include "../Dungeon.h"

#include "entity/environment/Door.h"

#include "loader/FileLoader.h"
#include "loader/InternalLoader.h"

#include "Pool.h"
#include "Floor.h"
#include "Options.h"

#include "entity/hero/Hero.h"
#include "entity/hero/Shot.h"
#include "entity/environment/Item.h"

#include "../peripheral/sound/Sound.h"
#include "../peripheral/led/Panel.h"
#include "../peripheral/Timer.h"

#include "animation/ExplosionAnimation.h"
#include "animation/VictoryAnimation.h"

void Room::update() {
    // update all entities
    // => the order does not matter
    Pool::update();

    // presort all entities so collision processing is faster
    // => usually nothing happens here, because apart from the first update, new entities are rare
    Pool::sort();

    // collision for all entities
    Pool::collision();

    Hero::reset((uint8_t) Hero::State::JUST_SPAWNED);
}

void Room::draw() {
    Pool::draw();
    Options::draw();
}

bool Room::load(uint8_t special) {
    LOG("load: ");
    LOG((int)Options::dungeon);
    LOG(" ");
    LOG((int)Floor::idx);
    LOG(" ");
    LOG_LN((int)Floor::room_idx);

    Hero::setGoal(false);
    Shot::reset();
    Pool::reset();
    bool was_options = Options::isOptions();
    Options::off();

    if (special == 1) {
        Options::setOptions();
    } else if (special == 2) {
        InternalLoader::load(255, 255);
        Options::setIdle();
    } else {
        bool success = false;
        if (Options::dungeon == 0) {
            // internal level
            success = InternalLoader::load(Floor::idx, Floor::room_idx);
        } else {
            // sdcard level
            success = FileLoader::load(Floor::idx, Floor::room_idx);
        }
        if (!success) {
            if ((Floor::idx == 0 && Floor::room_idx == 0) || was_options) {
                // fallback level
                Hero::spawn(0, 0);
            } else {
                return false;
            }
        }
    }
    return true;
}
