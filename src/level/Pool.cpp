#include "Pool.h"

#include "entity/Entity.h"

EntityUnion Pool::data[POOL_SIZE];

void Pool::reset() {
    for (uint8_t i = 0; i < POOL_SIZE; i++) {
        data[i].type = ENTITY_TYPE_COUNT;
    }
}

Entity *Pool::allocate(uint8_t type_id, bool force) {
    uint8_t max = POOL_SIZE;
    if (!force) {
      max -= 2;
    }
    for (uint8_t i = 0; i < max; i++) {
        if (data[i].type == ENTITY_TYPE_COUNT) {
            data[i].type = type_id;
            return (Entity*) &data[i];
        }
    }
    return nullptr;
}

void Pool::free(Entity *entity) {
    entity->type = ENTITY_TYPE_COUNT;
}

void Pool::update() {
    for (uint8_t idx = 0; idx < POOL_SIZE; idx++) {
        if (data[idx].isActive()) {
            data[idx].update();
        }
    }
}

void Pool::sort() {
    for (uint8_t ia = 0; ia < POOL_SIZE; ia++) {
        uint8_t type_a = data[ia].type;
        uint8_t swap_idx = ia;
        for (uint8_t ib = ia+1; ib < POOL_SIZE; ib++) {
            uint8_t type_b = data[ib].type;
            if (type_a > type_b) {
                swap_idx = ib;
                type_a = type_b;
            }
        }
        if (swap_idx != ia) {
            // swap
            uint8_t *ptr_a = (uint8_t*)&data[ia];
            uint8_t *ptr_b = (uint8_t*)&data[swap_idx];
            for (uint8_t i = 0; i < sizeof(EntityUnion); i++) {
                uint8_t tmp = ptr_a[i];
                ptr_a[i] = ptr_b[i];
                ptr_b[i] = tmp;
            }
        }
    }
}

void Pool::collision() {
    for (uint8_t ia = 0; ia < POOL_SIZE; ia++) {
        for (uint8_t ib = 0; ib < POOL_SIZE; ib++) {
            // recheck a every time, because it might have been destroyed and replaced
            uint8_t type_a = data[ia].type;
            uint8_t type_b = data[ib].type;
            if (type_a != ENTITY_TYPE_COUNT && type_b > type_a && data[ia].isActive() && data[ib].isActive()) {
                EntityUnion::collision(&data[ia], &data[ib]);
            }
        }
    }
}

void Pool::draw() {
  for (uint8_t idx = 0; idx < POOL_SIZE; idx++) {
    if (data[idx].isActive()) {
      data[idx].draw();
    }
  }
}

bool Pool::contains(uint8_t type_id) {
  for (uint8_t idx = 0; idx < POOL_SIZE; idx++) {
    if (data[idx].type == type_id && data[idx].isActive()) {
      return true;
    }
  }
  return false;
}

Entity *Pool::getFirst(uint8_t type_id) {
  for (uint8_t idx = 0; idx < POOL_SIZE; idx++) {
    if (data[idx].type == type_id && data[idx].isActive()) {
      return &data[idx].entity;
    }
  }
  return nullptr;
}
