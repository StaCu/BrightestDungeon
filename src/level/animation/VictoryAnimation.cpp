#include "VictoryAnimation.h"

#include <Platform.h>

#include "../Options.h"

#include "../../peripheral/sound/Sound.h"
#include "../../peripheral/led/Panel.h"
#include "../../util/Random.h"

#include "../Pool.h"

#include "../entity/hero/Hero.h"
#include "../entity/monster/Monster.h"

#include "ExplosionAnimation.h"
#include "../Floor.h"
#include "../Room.h"

#define MAX_COUNT (((MAX_ENTITY_COUNT-2)*4 > 255) ? 255 : ((MAX_ENTITY_COUNT-2)*4))

uint8_t VictoryAnimation::count = 0;

void VictoryAnimation::start() {
    Sound::play(Sound::VICTORY);
    count = MAX_COUNT;
}

void VictoryAnimation::update() {
    if (count > 1) {
        count -= 1;
        if ((count & 0b11) == 0) {
            Monster::spawn(0, 0, Random::get(32)+16);
        }
    } else if (count == 1 && !Pool::contains(MONSTER_ID)) {
        count = 0;
    }
}

void VictoryAnimation::draw() {
    if (VictoryAnimation::isActive()) {
        Panel::draw(Options::level_length-1, Hero::getColor());
    }
}

bool VictoryAnimation::isActive() {
    return count;
}
