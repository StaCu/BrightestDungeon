#include "Hero.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"
#include "../../animation/ExplosionAnimation.h"

#include "Sword.h"
#include "Shot.h"

#include "../monster/Monster.h"
#include "../monster/Boss.h"
#include "../monster/Worm.h"
#include "../monster/Spawner.h"

#include "../environment/Item.h"
#include "../environment/Door.h"
#include "../environment/ForceField.h"
#include "../environment/Lava.h"
#include "../environment/Mine.h"
#include "../environment/Portal.h"
#include "../environment/Water.h"
#include "../environment/Wall.h"

uint8_t Hero::state = 0;
uint8_t Hero::timeout = 0;
uint8_t Hero::checkpoint_pos = 0;

int16_t Hero::position = 0;
int8_t Hero::lives = 0;

void Hero::destroy() {
    Floor::loadFailure();
    Entity::destroy();
    if (isSet(State::JUST_SPAWNED)) {
        Hero::setCheckpointPos(255);
    }
}

Hero *Hero::spawn(uint8_t *args) {
  return Hero::spawn(args[0], Options::scalePosition(args[1]));
}

Hero *Hero::spawn(uint8_t mask, int16_t pos) {
    Hero *hero = (Hero*) Pool::allocate(HERO_ID);
    if (hero) {
        hero->MovingEntity::spawn(mask, pos);
        // set alive
        Hero::setAlive();
        Hero::set(State::JUST_SPAWNED);
        if (hero->isActive()) {
          if (checkpoint_pos <= 250) {
              hero->jump(Options::scalePosition(checkpoint_pos));
          } else {
              if (Floor::idx == 0 && !getColor()) {
                  // first level and nothing changed yet
                  // => if the player died, the live is returned
                  extraLive();
              }
          }
          Hero::position = hero->getPosition();
        }
        Hero::timeout = 1;

    }
    return hero;
}

void Hero::collision(Entity &entity, int16_t depth) {}
void Hero::collision(Monster &monster, int16_t depth) {
  this->destroy();
}
void Hero::collision(Boss &boss, int16_t depth) {
  this->destroy();
}
void Hero::collision(Worm &worm, int16_t depth) {
  this->destroy();
}
void Hero::collision(Door &door, int16_t depth) {
  if (Hero::isAttackStart()) {
    door.enter();
  }
}
void Hero::collision(ForceField &forcefield, int16_t depth) {
  if (getColor() != forcefield.getColor()) {
    this->destroy();
  }
}
void Hero::collision(Lava &lava, int16_t depth) {
  if (lava.isOn()) {
    this->destroy();
  }
}
void Hero::collision(Mine &mine, int16_t depth) {
  this->destroy();
}
void Hero::collision(Portal &portal, int16_t depth) {
  if (portal.isInverted()) {
    swapInverted();
  }

  int16_t dst = 0;
  if ((depth > 0) == portal.isInverted()) {
    // forwards
    dst = portal.getExit() - getSize();
  } else {
    // backwards
    dst = portal.getExit() + Options::size_medium;
  }
  jump(dst);
  Hero::position = getPosition();
  Sound::play(Sound::TELEPORT);
}
void Hero::collision(Switch &sw, int16_t depth) {
  if (Hero::isAttackStart()) {
    sw.use();
    Hero::resetStartAttack();
  }
}
void Hero::collision(Water &water, int16_t depth) {
  move(water.getCurrent());
  Hero::position = getPosition();
}
void Hero::collision(Wall &wall, int16_t depth) {
  jump(getPosition()-depth);
}
void Hero::collision(Item &item, int16_t depth) {
  switch (item.getType()) {
  case ItemType::SWAP_ATTACK_TYPE:
    swapAttackType();
    break;
  case ItemType::SET_COLOR_0:
    if (getColor() == 0) {
      return;
    }
    setColor(0);
    break;
  case ItemType::SET_COLOR_1:
    if (getColor() == 1) {
      return;
    }
    setColor(1);
    break;
  case ItemType::INVERT_CONTROL:
    swapInverted();
    break;
  case ItemType::EXTRA_LIFE:
    Hero::extraLive();
    break;
  }
  Sound::play(Sound::ITEM);
}

void Hero::update() {
  if (!isAlive()) {
    return;
  }
  MovingEntity::update();
  Hero::resetStartAttack();
  if (timeout) {
    if (timeout != 1 || !Controller::isAttack()) {
      timeout--;
    }
    return;
  }
  if (Controller::isAttack()) {
    Options::select();
    Hero::startAttack();
    if (Hero::isAttackTypeShoot()) {
        timeout = SHOOT_ATTACK_TIMEOUT_ITERATIONS;
        int16_t pos = getPosition();
        Shot::spawn(pos, -40);
        Shot::spawn(pos, 40);
    } else {
      timeout = SWORD_ATTACK_TIMEOUT_ITERATIONS;
      Sword::spawn();
    }
  } else {
    int16_t angle = Controller::getAngle();
    if (isInverted()) angle = 0 - angle;
    move(angle);
    Options::stopCombo();
  }
  Hero::position = getPosition();
  if (Hero::atGoal()) {
    Floor::loadSuccess();
  }
}

void Hero::draw() {
    Panel::draw(getPosition(), getColorIdx());
}

int16_t Hero::getSize() {
    return 1;
}

void Hero::setCheckpointPos(uint8_t pos) {
    Hero::checkpoint_pos = pos;
}

bool Hero::isAlive() {
    return Hero::state & State::IS_ALIVE;
}

void Hero::setAlive() {
    Hero::state = (State) (state | State::IS_ALIVE);
}

void Hero::extraLive() {
    if (Hero::lives < MAX_LIVES) {
        Hero::lives += 1;
    }
}

void Hero::looseLive() {
    if (Hero::lives > 0) {
        Hero::lives -= 1;
    }
    Hero::state = (State) (state & ~State::IS_ALIVE);
}

void Hero::setGoal(bool goal) {
    if (goal) {
        Hero::state = (State) (state | State::HAS_GOAL);
    } else {
        Hero::state = (State) (state & ~State::HAS_GOAL);
    }
}

bool Hero::hasGoal() {
  return (state & State::HAS_GOAL);
}

bool Hero::atGoal() {
    if (Hero::hasGoal() && Hero::position == Options::level_length-1 && !Pool::contains(BOSS_ID)) {
        return true;
    } else {
        return false;
    }
}

void Hero::setColor(uint8_t color) {
    if (color) {
        Hero::state = (State) (state | State::IS_COLOR1);
    } else {
        Hero::state = (State) (state & ~State::IS_COLOR1);
    }
}

bool Hero::getColor() {
    return (state & State::IS_COLOR1) ? true : false;
}

uint8_t Hero::getColorIdx() {
    if (getColor() != Options::swap_color) return COLOR_HERO_1;
    else return COLOR_HERO_0;
}

void Hero::startAttack() {
    Hero::state = (State) (state | State::IS_ATTACK_START);
}

void Hero::resetStartAttack() {
    Hero::state = (State) (state & ~State::IS_ATTACK_START);
}

bool Hero::isAttackStart() {
    return state & State::IS_ATTACK_START;
}

void Hero::swapAttackType() {
    Hero::state = (State) (state ^ State::HAS_SHOT);
}

void Hero::resetAttackType() {
    Hero::state = (State) (state & ~State::HAS_SHOT);
}

bool Hero::isAttackTypeShoot() {
    return state & State::HAS_SHOT;
}

void Hero::swapInverted() {
    Hero::state = (State) (state ^ State::IS_INVERTED);
}

bool Hero::isInverted() {
    return state & State::IS_INVERTED;
}

void Hero::resetFailure() {
    reset(State::RESET_FAILURE);
}

void Hero::resetSuccess() {
    if (Hero::getColor()) {
        Options::swap_color = !Options::swap_color;
    }
    reset(State::RESET_SUCCESS);
    Hero::setCheckpointPos(255);
}

void Hero::resetDefeat() {
    reset(State::RESET_DEFEAT);
    Hero::lives = MAX_LIVES;
    Hero::setCheckpointPos(255);
}

void Hero::set(uint8_t bit) {
  Hero::state = (State) (state | bit);
}

void Hero::reset(uint8_t bit) {
  Hero::state = (State) (state & ~bit);
}

void Hero::swap(uint8_t bit) {
  Hero::state = (State) (state ^ bit);
}

bool Hero::isSet(uint8_t bit) {
  return (Hero::state & bit) != 0;
}
