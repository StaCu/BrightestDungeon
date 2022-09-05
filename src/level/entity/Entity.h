#pragma once

#include <Platform.h>

#define MAX_PARAMETER_PER_ENTITY 5

enum EntityType : uint8_t {
    WATER_ID,
    PORTAL_ID,
    FORCEFIELD_ID,
    LAVA_ID,
    WALL_ID,
    SWORD_ID,
    SHOT_ID,
    HERO_ID,
    BOSS_ID,
    WORM_ID,
    DOOR_ID,
    BUTTON_ID,
    SWITCH_ID,
    ITEM_ID,
    MINE_ID,
    SPAWNER_ID,
    FOG_ID,
    MONSTER_ID, // monsters come last, because they can respawn (this improves sorting speed)

    // -----
    ENTITY_TYPE_COUNT,
    
    RESET_ID,
    END_ID,
    NEXT_ID,
    SECRET_ID,
};

/**
 * DOOR (to same room) & PORTAL
 * => move the player within the same room
 *    DOOR: player has the choice to teleport
 *    PORTAL: player has no choice
 * 
 * SWITCH & BUTTON
 * => change the mask
 *    SWITCH: player has the choice to use it
 *    BUTTON: player has no choice
 */

/**
 * Entity is the base class of all objects that
 * exist inside the dungeon.
 *
 * the packed attribute is important, so the type attributes remain the first byte of
 * each entity and no bytes are wasted.
 */
class __attribute__ ((packed)) Entity {

private:

  /**
   * type is a 8 bit typeID unique to every fully implemented
   * subclass of Entity
   *
   * it is used by the @link{src/util/Pool.h} to select the
   * appropriate member function when operating on an Entity
   *
   * type == ENTITY_TYPE_COUNT signals that the entity does not exist anymore
   * this allows for a save removal of that entity from a data
   * structure
   */
  uint8_t type;

  uint8_t mask;

protected:
  /**
   * the minimum position of the entity
   */
  int16_t pos;

public:

  static uint8_t global_mask;

  static Entity *spawn(uint8_t type, uint8_t *args);

  static void resetMask();

  static void resetMaskPartially(uint8_t mask);

  void spawn(uint8_t mask, int16_t pos);

  void destroy();

  void update();

  bool isActive() const;

  void draw();

  uint8_t getMask();

  void jump(int16_t pos);

  uint8_t getType() const;

  int16_t getSize();

  int16_t getPosition();

  int16_t getOldPosition();

  /**
   * returns the number of arguments required to use the
   * spawn(uint8_t* args) constructor of a specific entity type.
   *
   * spawning an entity using the spawn(uint8_t* args) constructor
   * requires a certain amount of arguments, a number, which may not be available
   * to the caller (e.g the FileIO doesn't know how many arguments to
   * read).
   *
   * this method carries a lookup table which returns the corresponding
   * number. adding a new entity requires to update this method as well.
   *
   * @param type
   *    the entity type
   *
   * @return
   *    the number of required arguments
   */
  static uint8_t getArgCount(uint8_t type);

  friend class Pool;
};
