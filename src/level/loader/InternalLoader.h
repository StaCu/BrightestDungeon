#pragma once

#include <stdint.h>

/**
 * InternalLoader is a static class, which manages loading of room data
 * from program memory into the pool.
 *
 * rooms are saved inside PROGMEM as an array of 8 bit values
 * and are referenced by their index, which is mostly relevant
 * for the load(...) method.

 * For spawning entities, the elements are interpreted as
 * follows:
 * typeID, arg 0, arg 1, ...,
 * typeID, arg 0, ...,
 * ...
 * the sequence is terminated by a special typeID
 */
class InternalLoader {

  /**
   * for easier extendability, the level data is declared and defined in the .cpp file
   * and not here as well. 
   */

public:

  /**
   * loads a new room.
   * calls Entity::spawn(...) for every entity in the new room
   *
   * all rooms that are in the internal storage have a hardcoded
   * index, therefore adding a new room requires to add it to
   * this methods lookup table as well.
   *
   * @param floor
   *     the floor
   * @param room
   *     the room
   */
  static bool load(uint8_t  floor, uint8_t room);

};
