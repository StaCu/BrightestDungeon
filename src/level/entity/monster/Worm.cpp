#include "Worm.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"
#include "../../../util/Random.h"
#include "../../../util/Math.h"

#include "../hero/Sword.h"
#include "../hero/Shot.h"
#include "../environment/ForceField.h"
#include "../environment/Lava.h"
#include "../environment/Water.h"
#include "../environment/Wall.h"

Worm *Worm::spawn(uint8_t *args) {
    return Worm::spawn(args[0], Options::scalePosition(args[1]), args[2]);
}

Worm *Worm::spawn(uint8_t mask, int16_t pos, int8_t speed) {
    Worm *worm = (Worm*) Pool::allocate(WORM_ID);
    if (worm) {
        worm->MovingEntity::spawn(mask, pos);
        worm->speed = speed;
        worm->contracting_and_length = 0b10000000 | Options::size_large;
        worm->cut = 0;
    }
    return worm;
}

void Worm::update() {
    MovingEntity::update();

    uint8_t contracting = contracting_and_length & 0b10000000;
    int8_t length = contracting_and_length & 0b01111111;

    bool move_onward = (speed > 0) == (contracting != 0);
    int8_t tmp_speed = speed;
    if (!contracting) {
        // the expanding phase should be quick/surprising
        tmp_speed *= 4;
    }

    int16_t old_pos = pos;
    int8_t side = move(tmp_speed);
    if (side) {
        speed = 0 - speed;
    }
    if (contracting) {
        length -= math::abs16(old_pos - pos);
        if (length < Options::size_small - cut/4) {
            // note:
            // -cut/4 has the side effect that
            // WORM_SIZE_EXPANDED == 4*WORM_SIZE_CONTRACTED
            length = Options::size_small - cut/4;
            contracting = 0;
        }
    } else {
        length += math::abs16(old_pos - pos);
        if (length > Options::size_large - cut) {
            length = Options::size_large - cut;
            contracting = 0b10000000;
        }
    }
    if (!move_onward) {
        pos = old_pos;
    }
    contracting_and_length = contracting | length;
}

void Worm::collision_turn(int16_t depth) {
    jump(getPosition()-depth);
    speed = 0 - speed;
}

void Worm::collision_harm(int16_t depth) {
    int8_t length = contracting_and_length & 0b01111111;
    uint8_t contracting = contracting_and_length & 0b10000000;
    length -= math::abs16(depth);
    cut += math::abs16(depth);
    if (length <= 0) {
        this->destroy();
        Sound::play(Sound::ATTACK);
    } else if (depth < 0) {
        jump(getPosition()-depth);
    }
    contracting_and_length = contracting | length;
}

void Worm::collision(Entity &entity, int16_t depth) {}

void Worm::collision(Sword &sword, int16_t depth) {
    collision_harm(depth);
}

void Worm::collision(Shot &shot, int16_t depth) {
    if (depth < 0) depth = -3;
    else           depth = 3;
    collision_harm(depth);
}

void Worm::collision(ForceField &forcefield, int16_t depth) {
    collision_turn(depth);
}

void Worm::collision(Lava &lava, int16_t depth) {
    collision_turn(depth);
}

void Worm::collision(Water &water, int16_t depth) {
    collision_turn(depth);
}

void Worm::collision(Wall &wall, int16_t depth) {
    collision_turn(depth);
}

void Worm::draw() {
    uint8_t flicker = Random::get(3);    
    bool contracting = contracting_and_length & 0b10000000;
    int8_t length = contracting_and_length & 0b01111111;
    int16_t pos  = getPosition();
    if (!flicker && contracting) {
        length--;
        if (speed > 0) pos++;
    }
    Panel::drawLine(pos, length, contracting ? COLOR_MONSTER_0 : COLOR_MONSTER_1);
}

int16_t Worm::getSize() {
    return contracting_and_length & 0b01111111;
}
