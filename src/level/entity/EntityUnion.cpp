#include "EntityUnion.h"

#include "Entity.h"
#include "LongEntity.h"
#include "MovingEntity.h"
#include "hero/Hero.h"
#include "hero/Shot.h"
#include "hero/Sword.h"
#include "monster/Boss.h"
#include "monster/Monster.h"
#include "monster/Spawner.h"
#include "monster/Worm.h"
#include "environment/Button.h"
#include "environment/Door.h"
#include "environment/Fog.h"
#include "environment/ForceField.h"
#include "environment/Item.h"
#include "environment/Lava.h"
#include "environment/Mine.h"
#include "environment/Portal.h"
#include "environment/Switch.h"
#include "environment/Wall.h"
#include "environment/Water.h"

EntityUnion::EntityUnion() {
    type = ENTITY_TYPE_COUNT;
    sizeof(EntityUnion);
    sizeof(Entity);
    sizeof(LongEntity);
    sizeof(MovingEntity);

    sizeof(Hero);
    sizeof(Shot);
    sizeof(Sword);

    sizeof(Boss);
    sizeof(Monster);
    sizeof(Spawner);
    sizeof(Worm);
    
    sizeof(Button);
    sizeof(Door);
    sizeof(Fog);
    sizeof(ForceField);
    sizeof(Item);
    sizeof(Lava);
    sizeof(Mine);
    sizeof(Portal);
    sizeof(Switch);
    sizeof(Wall);
    sizeof(Water);
}

bool EntityUnion::isActive() const {
    return entity.isActive();
}

void EntityUnion::update() {
    switch (this->type) {
    case HERO_ID:       hero.update(); break;
    case SHOT_ID:       shot.update(); break;
    case SWORD_ID:      sword.update(); break;

    case BOSS_ID:       boss.update(); break;
    case MONSTER_ID:    monster.update(); break;
    case WORM_ID:       worm.update(); break;
    case SPAWNER_ID:    spawner.update(); break;

    case BUTTON_ID:     button.update(); break;
    case DOOR_ID:       door.update(); break;
    case FOG_ID:        fog.update(); break;
    case FORCEFIELD_ID: forcefield.update(); break;
    case ITEM_ID:       item.update(); break;
    case LAVA_ID:       lava.update(); break;
    case MINE_ID:       mine.update(); break;
    case PORTAL_ID:     portal.update(); break;
    case SWITCH_ID:     sw.update(); break;
    case WALL_ID:       wall.update(); break;
    case WATER_ID:      water.update(); break;
    }
}

void EntityUnion::draw() {
    switch (this->type) {
    case HERO_ID:       hero.draw(); break;
    case SHOT_ID:       shot.draw(); break;
    case SWORD_ID:      sword.draw(); break;

    case BOSS_ID:       boss.draw(); break;
    case MONSTER_ID:    monster.draw(); break;
    case WORM_ID:       worm.draw(); break;
    case SPAWNER_ID:    spawner.draw(); break;

    case BUTTON_ID:     button.draw(); break;
    case DOOR_ID:       door.draw(); break;
    case FOG_ID:        fog.draw(); break;
    case FORCEFIELD_ID: forcefield.draw(); break;
    case ITEM_ID:       item.draw(); break;
    case LAVA_ID:       lava.draw(); break;
    case MINE_ID:       mine.draw(); break;
    case PORTAL_ID:     portal.draw(); break;
    case SWITCH_ID:     sw.draw(); break;
    case WALL_ID:       wall.draw(); break;
    case WATER_ID:      water.draw(); break;
    }
}


#define MIX_AB(a, b) (((uint16_t) a << 8) | (uint16_t) b)
#define MIX_BA(a, b) ((uint16_t) a | ((uint16_t) b << 8))
#define MIX_SL(a, b) ((a > b) ? ((uint16_t) a | ((uint16_t) b << 8)) : ((uint16_t) b | ((uint16_t) a << 8)))
#define COLLISION(TYPEA, TYPEB) a->TYPEA.collision(b->TYPEB, depth); b->TYPEB.collision(a->TYPEA, -depth); break
void EntityUnion::collision(EntityUnion *a, EntityUnion *b) {
  int16_t depth = EntityUnion::getCollisionDepth(
    a->getPosition(), a->getSize(), a->getOldPosition(),
    b->getPosition(), b->getSize(), b->getOldPosition()
  );
  if (depth == 0) return;

  switch (MIX_AB(a->type, b->type)) {
  case MIX_SL(WALL_ID, HERO_ID):          COLLISION(wall, hero);
  case MIX_SL(WALL_ID, MONSTER_ID):       COLLISION(wall, monster);
  case MIX_SL(WALL_ID, WORM_ID):          COLLISION(wall, worm);
  case MIX_SL(WALL_ID, SWORD_ID):         COLLISION(wall, sword);
  case MIX_SL(WALL_ID, SHOT_ID):          COLLISION(wall, shot);

  case MIX_SL(HERO_ID, BOSS_ID):          COLLISION(hero, boss);
  case MIX_SL(HERO_ID, BUTTON_ID):        COLLISION(hero, button);
  case MIX_SL(HERO_ID, MONSTER_ID):       COLLISION(hero, monster);
  case MIX_SL(HERO_ID, WORM_ID):          COLLISION(hero, worm);
  case MIX_SL(HERO_ID, DOOR_ID):          COLLISION(hero, door);
  case MIX_SL(HERO_ID, FORCEFIELD_ID):    COLLISION(forcefield, hero);
  case MIX_SL(HERO_ID, ITEM_ID):          COLLISION(hero, item);
  case MIX_SL(HERO_ID, LAVA_ID):          COLLISION(lava, hero);
  case MIX_SL(HERO_ID, MINE_ID):          COLLISION(hero, mine);
  case MIX_SL(HERO_ID, PORTAL_ID):        COLLISION(portal, hero);
  case MIX_SL(HERO_ID, SWITCH_ID):        COLLISION(hero, sw);
  case MIX_SL(HERO_ID, WATER_ID):         COLLISION(water, hero);

  case MIX_SL(SWORD_ID, BOSS_ID):         COLLISION(sword, boss);
  case MIX_SL(SWORD_ID, MONSTER_ID):      COLLISION(sword, monster);
  case MIX_SL(SWORD_ID, WORM_ID):         COLLISION(sword, worm);
  case MIX_SL(SWORD_ID, FORCEFIELD_ID):   COLLISION(forcefield, sword);
  case MIX_SL(SWORD_ID, MINE_ID):         COLLISION(sword, mine);

  case MIX_SL(SHOT_ID, BOSS_ID):          COLLISION(shot, boss);
  case MIX_SL(SHOT_ID, MONSTER_ID):       COLLISION(shot, monster);
  case MIX_SL(SHOT_ID, WORM_ID):          COLLISION(shot, worm);
  case MIX_SL(SHOT_ID, FORCEFIELD_ID):    COLLISION(forcefield, shot);
  case MIX_SL(SHOT_ID, LAVA_ID):          COLLISION(lava, shot);
  case MIX_SL(SHOT_ID, MINE_ID):          COLLISION(shot, mine);

  case MIX_SL(MONSTER_ID, FORCEFIELD_ID): COLLISION(forcefield, monster);
  case MIX_SL(MONSTER_ID, LAVA_ID):       COLLISION(lava, monster);
  case MIX_SL(MONSTER_ID, MINE_ID):       COLLISION(mine, monster);

  case MIX_SL(WORM_ID, FORCEFIELD_ID):    COLLISION(forcefield, worm);
  case MIX_SL(WORM_ID, LAVA_ID):          COLLISION(lava, worm);
  case MIX_SL(WORM_ID, MINE_ID):          COLLISION(worm, mine);
  case MIX_SL(WORM_ID, WATER_ID):         COLLISION(water, worm);
  }
}

int16_t EntityUnion::getCollisionDepth(int16_t a_pos, int16_t a_size, int16_t a_old_pos, int16_t b_pos, int16_t b_size,
                                 int16_t b_old_pos) {
  int16_t depth = 0;
  bool invert = false;
  if (a_pos < b_pos) {
    if (a_old_pos < b_old_pos) {
      depth = a_pos + a_size - b_pos;
      // positive in case of collision
      // => a.pos - depth would solve the collision
    } else {
      // both crossed over each other
      depth = b_pos + b_size - a_pos;
      // positive in case of collision
      // => a.pos + depth would solve the collision
      invert = true;
    }
  } else {
    if (a_old_pos > b_old_pos) {
      depth = b_pos + b_size - a_pos;
      // positive in case of collision
      // => a.pos + depth would solve the collision
      invert = true;
    } else {
      // both crossed over each other
      depth = a_pos + a_size - b_pos;
      // positive in case of collision
      // => a.pos - depth would solve the collision
    }
  }

  if (depth < 0) {
    // no collision
    return 0;
  }

  if (invert) return 0 - depth;
  else return depth;
}

int16_t EntityUnion::getPosition() {
  if (type == SWORD_ID) {
    return sword.getPosition();
  }
  return entity.getPosition();
}

int16_t EntityUnion::getSize() {
  switch (this->type) {
    case HERO_ID:       return hero.getSize(); break;
    case SHOT_ID:       return shot.getSize(); break;
    case SWORD_ID:      return sword.getSize(); break;

    case BOSS_ID:       return boss.getSize(); break;
    case MONSTER_ID:    return monster.getSize(); break;
    case WORM_ID:       return worm.getSize(); break;
    //case SPAWNER_ID:  return .->getSize(); break;

    case BUTTON_ID:     return button.getSize(); break;
    case DOOR_ID:       return door.getSize(); break;
    case FOG_ID:        return fog.getSize(); break;
    case FORCEFIELD_ID: return forcefield.getSize(); break;
    case ITEM_ID:       return item.getSize(); break;
    case LAVA_ID:       return lava.getSize(); break;
    case MINE_ID:       return mine.getSize(); break;
    case PORTAL_ID:     return portal.getSize(); break;
    case SWITCH_ID:     return sw.getSize(); break;
    case WALL_ID:       return wall.getSize(); break;
    case WATER_ID:      return water.getSize(); break;
  default: return 0;
  }
}

int16_t EntityUnion::getOldPosition() {
  switch (this->type) {
    case HERO_ID:       return hero.getOldPosition(); break;
    case SHOT_ID:       return shot.getOldPosition(); break;
    case SWORD_ID:      return sword.getOldPosition(); break;

    case BOSS_ID:       return boss.getOldPosition(); break;
    case MONSTER_ID:    return monster.getOldPosition(); break;
    case WORM_ID:       return worm.getOldPosition(); break;
    case SPAWNER_ID:    return spawner.getOldPosition(); break;

    case BUTTON_ID:     return button.getOldPosition(); break;
    case DOOR_ID:       return door.getOldPosition(); break;
    case FOG_ID:        return fog.getOldPosition(); break;
    case FORCEFIELD_ID: return forcefield.getOldPosition(); break;
    case ITEM_ID:       return item.getOldPosition(); break;
    case LAVA_ID:       return lava.getOldPosition(); break;
    case MINE_ID:       return mine.getOldPosition(); break;
    case PORTAL_ID:     return portal.getOldPosition(); break;
    case SWITCH_ID:     return sw.getOldPosition(); break;
    case WALL_ID:       return wall.getOldPosition(); break;
    case WATER_ID:      return water.getOldPosition(); break;
    default: return getPosition();
  }
}
