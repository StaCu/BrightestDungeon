#pragma once

#include "entity/EntityUnion.h"

class Entity;

/**
 * the main data struct holding every entity
 * that is active inside the current level
 *
 * it allows for dynamic allocation of entities
 * as well as semi-virtual method calls
 */
struct Pool {

private:
    /**
     * holds the data of all entities
     */
    static EntityUnion data[];

public:

    static void reset();

    static Entity *allocate(uint8_t type_id, bool force = false);

    static void free(Entity *entity);

    static void sort();

    static void update();

    static void collision();

    static void draw();

    /**
     * returns whether at least one entity of the given type is active
     *
     * @param type_id
     *   the type
     *
     * @return
     *   whether an entity of that type exists and is active
     */
    static bool contains(uint8_t type_id);

    /**
     * returns the first entity of the given type that is active
     *
     * @param type_id
     *    the type
     *
     * @return
     *    the first entity of that type that is active
     *    or
     *    nullptr
     */
    static Entity *getFirst(uint8_t type_id);

};
