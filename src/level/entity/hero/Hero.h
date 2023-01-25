#pragma once

#include "../MovingEntity.h"
class Sword;
class Shot;
class Boss;
class Monster;
class Worm;
class Spawner;
class Item;
class Door;
class ForceField;
class Lava;
class Mine;
class Portal;
class Switch;
class Water;
class Wall;

#define SWORD_ATTACK_TIMEOUT_ITERATIONS 32
#define SHOOT_ATTACK_TIMEOUT_ITERATIONS 12

class __attribute__ ((packed)) Hero : public MovingEntity {

private:

  static uint8_t state;
  static uint8_t checkpoint_pos;
  static uint8_t timeout;

public:

  enum State : uint8_t {
    HAS_GOAL        = 0b00000001,
    IS_ATTACK_START = 0b00000010,
    HAS_SHOT        = 0b00000100,
    IS_INVERTED     = 0b00001000,
    IS_ALIVE        = 0b00010000,
    IS_COLOR1       = 0b00100000,
    JUST_SPAWNED    = 0b01000000,

    RESET_FAILURE   = 0b01000100,
    RESET_SUCCESS   = 0b01101000,
    RESET_DEFEAT    = 0b01101100,
  };

  static int8_t lives;
  static int16_t position;

  void destroy();

  static Hero *spawn(uint8_t *args);

  static Hero *spawn(uint8_t mask, int16_t pos);

  void collision(Entity &entity, int16_t depth);

  void collision(Monster &monster, int16_t depth);

  void collision(Boss &boss, int16_t depth);

  void collision(Worm &worm, int16_t depth);

  void collision(Door &door, int16_t depth);

  void collision(ForceField &forcefield, int16_t depth);

  void collision(Lava &lava, int16_t depth);

  void collision(Mine &mine, int16_t depth);

  void collision(Portal &portal, int16_t depth);

  void collision(Switch &sw, int16_t depth);

  void collision(Water &water, int16_t depth);

  void collision(Wall &wall, int16_t depth);

  void collision(Item &item, int16_t depth);

  void update();

  void draw();

// # ================================================================ #
// # ::  GETTER / SETTER                                           :: #
// # ================================================================ #

  int16_t getSize();

  static void setCheckpointPos(uint8_t pos);

  static bool isAlive();

  static void setAlive();

  static void extraLive();

  static void looseLive();

  static void setGoal(bool goal);

  static bool hasGoal();

  static bool atGoal();

  static void setColor(uint8_t color);

  static bool getColor();

  static uint8_t getColorIdx();

  static void startAttack();

  static void resetStartAttack();

  static bool isAttackStart();

  static void swapAttackType();

  static void resetAttackType();

  static bool isAttackTypeShoot();

  static void resetJustSpawned();

  static void swapInverted();

  static bool isInverted();

  static void resetFailure();

  static void resetSuccess();

  static void resetDefeat();

  static void set(uint8_t bit);

  static void reset(uint8_t bit);

  static void swap(uint8_t bit);

  static bool isSet(uint8_t bit);

};
