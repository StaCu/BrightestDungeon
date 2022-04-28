#include "Spawner.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "Monster.h"

Spawner *Spawner::spawn(uint8_t *args) {
    return Spawner::spawn(args[0], args[1], args[2], Options::scalePosition(args[3]), args[4]);
}

Spawner *Spawner::spawn(uint8_t mask, uint8_t first, uint8_t interval, int16_t pos, int8_t speed) {
    Spawner *spawner = (Spawner*) Pool::allocate(SPAWNER_ID);
    if (spawner) {
      spawner->Entity::spawn(mask, pos);
      spawner->timer = Timer(first, interval);
      spawner->speed = speed;
    }
    return spawner;
}

void Spawner::update() {
    if (timer.dec() && abs(Hero::position - pos) > Options::size_large) {
        // the timer reached zero
        // => spawn a new monster
        // => but only if the player isn't too close
        Monster::spawn(0, getPosition(), speed);
    }
}
