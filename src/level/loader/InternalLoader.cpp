#include "InternalLoader.h"

#include "../../Settings.h"

#include "../Floor.h"
#include "../Room.h"

#include "../entity/EntityUnion.h"

#include "../../peripheral/sound/Sound.h"

// # ================================================================ #
// # ::  SHORTCUTS                                                 :: #
// # ================================================================ #

#define HERO(mask, pos)                        HERO_ID,       mask, pos,
#define BOSS(mask,pos1,pos2,pos3)              BOSS_ID,       mask, pos1, pos2, pos3,
#define MONSTER(mask,pos,speed)                MONSTER_ID,    mask, pos, (uint8_t) (speed),
#define WORM(mask,pos,speed)                   WORM_ID,       mask, pos, (uint8_t) (speed),
#define SPAWNER(mask,t0,t1,pos,speed)          SPAWNER_ID,    mask, (t0), (t1), pos, (uint8_t) (speed),
#define BUTTON(mask,pos,op,t0,t1)              BUTTON_ID,     mask, pos, op, (t0), (t1),
#define DOOR(mask,pos,lvl,dst)                 DOOR_ID,       mask, pos, lvl, dst,
#define SWITCH(mask,pos,op)                    SWITCH_ID,     mask, pos, op,
#define FOG(mask,pos,dim,speed)                FOG_ID,        mask, pos, dim, (uint8_t) (speed),
#define FORCEFIELD(mask,pos,color)             FORCEFIELD_ID, mask, pos, color,
#define ITEM(mask,pos,type)                    ITEM_ID,       mask, pos, (uint8_t) type,
#define LAVA(mask,pos,dim,t0,t1)               LAVA_ID,       mask, pos, dim, (t0), (t1),
#define MINE(mask,pos)                         MINE_ID,       mask, pos,
#define PORTAL(mask,pos,dst)                   PORTAL_ID,     mask, pos, dst,
#define PORTAL2(mask,pos,dst)
#define WALL(mask,pos,dim)                     WALL_ID,       mask, pos, dim,
#define WATER(mask,pos,dim,current)            WATER_ID,      mask, pos, dim, (uint8_t) (current),
#define RESET(mask)                            RESET_ID,      mask,
#define END END_ID,
#define NEXT NEXT_ID,
#define SECRET SECRET_ID,

#define ROOM(f,r) const PROGMEM uint8_t F ## f ## R ## r[] =

#if ENABLE_INTERNAL_DUNGEON

// # ================================================================ #
// # ::  DATA                                                      :: #
// # ================================================================ #

/**
 * first up:
 * red = bad
 * green = good
 */
ROOM(0,0) {
    // Introduction - MOVE, ATTACK, GOAL, MONSTER
    MONSTER    (0,0,0)
    HERO       (0, 50)
    MONSTER    (0,200,0)
    NEXT
};

/**
 * Forcefields and doors are what can turn this
 * game into a small puzzle game
 * => introduce them early
 */
ROOM(1,0) {
    // Introduction - FORCEFIELD
    HERO       (0,0)
    ITEM       (0,50,1)
    FORCEFIELD (0,100,1)
    MONSTER    (0,125,0)
    ITEM       (0,150,0)
    FORCEFIELD (0,200,0)
    MONSTER    (0,225,0)
    NEXT
};

/**
 * This level gives an introduction to doors
 * in combination with the previous level, the player
 * "should" have the idea that passing a forcefield
 * with a different color is not possible
 * (if not, the player will know this after trying :P)
 * The doors are the only way around the forcefield,
 * so the player will figure it out eventually.
 */
ROOM(2,0) {
    // Introduction - DOOR
    HERO       (0,0)
    MONSTER    (0,25,2)
    DOOR       (0,100, 1, 100)
    MONSTER    (0,140,-2)
    FORCEFIELD (0,150,1)
    ITEM       (0,160,4)
    MONSTER    (0,175,2)
    DOOR       (0,200, 1, 200)
    ITEM       (0,225,1)
    SPAWNER    (0, 0, 80, 250, -2)
    NEXT
};
ROOM(2,1) {
    // Introduction - DOOR2
    ITEM       (0,5,2)
    FORCEFIELD (0,25,1)
    HERO       (0,0)
    DOOR       (0,100, 0, 100)
    DOOR       (0,200, 0, 200)
    END
};

/**
 * full assault
 */
ROOM(3,0) {
    HERO       (0,0)
    MONSTER    (0,40,0)
    MONSTER    (0,50,-8)
    MONSTER    (0,60,8)
    MONSTER    (0,70,0)
    MONSTER    (0,80,-8)
    MONSTER    (0,90,8)
    WORM       (0,110,-8)
    WORM       (0,120,8)
    MONSTER    (0,140,0)
    MONSTER    (0,150,-8)
    MONSTER    (0,160,8)
    SPAWNER    (0, 0, 80, 250, -4)
    SPAWNER    (0, 10, 80, 250, -8)
    NEXT
};

/**
 * introduction to switches
 */
ROOM(4,0) {
    // Introduction - SWITCH
    HERO       (0,0)
    SWITCH     (0,30, 0b00010001)
    DOOR       (0b00000010, 80, 1, 80)
    FORCEFIELD (0b00010000,100,1)
    SWITCH     (0,175, 0b00010001)
    FORCEFIELD (0b00000001,200,1)
    BUTTON     (0b00100000,220, 0b00100010, 0, 0)
    SPAWNER    (0, 0, 80, 250, -2)
    NEXT
};
ROOM(4,1) {
    HERO       (0, 0)
    DOOR       (0, 80, 0, 80)
    DOOR       (0, 150, 1, 190)
    WALL       (0, 160, 20)
    DOOR       (0, 190, 1, 150)
    LAVA       (0, 200, 20, 0, 20)
    ITEM       (0, 240, 4)
    END
};

/**
 * this level has a forcefield. To get the correct color
 * the player needs to go through a door, clear the level
 * and return
 */
ROOM(5,0) {
    HERO       (0,0)
    MONSTER    (0,35,-2)
    MONSTER    (0,35,2)
    DOOR       (0,50, 1, 50)
    FORCEFIELD (0,100,1)
    MONSTER    (0,110,2)
    MONSTER    (0,140,2)
    ITEM       (0,150,4)
    NEXT
};
ROOM(5,1) {
    HERO       (0,0)
    DOOR       (0,50, 0, 50)
    LAVA       (0,125,25,0,30)
    LAVA       (0,175,25,0,30)
    ITEM       (0,225,1)
    SPAWNER    (0,0, 80, 250, -3)
    SPAWNER    (0,10, 80, 250, -15)
    END
};

/**
 * introduction to water
 * this level has a waters stream followed by
 * a monster so you need to careful about
 * speed
 * after that the player is forced through
 * a door which leads to two consecutive levels
 * before reentering the original level through
 * the final door
 */
ROOM(6,0) {
    HERO       (0,0)
    WATER      (0,25, 75, -20)
    DOOR       (0,110, 1, 110)
    FORCEFIELD (0,135,1)
    WORM       (0,150,10)
    SPAWNER    (0,0, 40, 250, -5)
    NEXT
};
ROOM(6,1) {
    HERO       (0,0)
    SPAWNER    (0,40, 40, 0, 5)
    DOOR       (0,30,2,30)
    WATER      (0,40,60,-10)
    MONSTER    (0,50,0)
    MONSTER    (0,60,0)
    MONSTER    (0,70,0)
    DOOR       (0,110,0,110)
    WATER      (0,130,60,10)
    MONSTER    (0,150,0)
    MONSTER    (0,160,0)
    MONSTER    (0,170,0)
    DOOR       (0,200,2,200)
    SPAWNER    (0,40, 40, 250, -5)
    END
};
ROOM(6,2) {
    HERO       (0,0)
    DOOR       (0b00010000,30,1,30)
    BUTTON     (0b00110000,40,0b00010000,0,0)
    WATER      (0,60,50,-10)
    MONSTER    (0,120,0)
    ITEM       (0,130,1)
    ITEM       (0,140,4)
    WORM       (0,150,10)
    BUTTON     (0b00110000,190,0b00100000,0,0)
    DOOR       (0b00100000,200,1,200)
    END
};

/**
 * first labyrinth
 */
ROOM(7,0) {
    HERO       (0,0)
    DOOR       (0,30,1,30)
    LAVA       (0,60,70,20,30)
    DOOR       (0,150,2,150)
    MONSTER    (0,180,0)
    MONSTER    (0,160,8)
    END
};
ROOM(7,1) {
    HERO       (0,0)
    DOOR       (0,30,0,30)
    DOOR       (0,100,2,100)
    WALL       (0,110,30)
    DOOR       (0,150,3,150)
    DOOR       (0,180,4,180)
    FORCEFIELD (0,230,1)
    NEXT
};
ROOM(7,2) {
    HERO       (0,0)
    DOOR       (0,20,4,20)
    WATER      (0,30,60,-10)
    MONSTER    (0,50,0)
    DOOR       (0,100,1,100)
    WALL       (0,110,30)
    DOOR       (0,150,0,150)
    ITEM       (0,180,1)
    END
};
ROOM(7,3) {
    HERO       (0,0)
    DOOR       (0,30,0,30)
    LAVA       (0,50,30,0,40)
    MONSTER    (0,90,0)
    LAVA       (0,100,30,20,40)
    DOOR       (0,150,1,150)
    MONSTER    (0,190,0)
    MONSTER    (0,200,0)
    ITEM       (0,220,4)
    SPAWNER    (0,10,40,250,-10)
    END
};
ROOM(7,4) {
    HERO       (0,0)
    DOOR       (0,20,2,20)
    DOOR       (0,180,1,180)
    END
};

/**
 * this level is about timing and taking the risk.
 * the lava is timed so going full speed will just get the
 * player through the level in one go.
 * Additionally, the spawner is timed so the monsters will make it pretty
 * far.
 */
ROOM(8,0) {
    HERO       (0,0)
    DOOR       (0,30,1,30)
    LAVA       (0,50,25,40,40)
    LAVA       (0,100,25,40,40)
    LAVA       (0,150,25,40,40)
    MONSTER    (0,175,0)
    LAVA       (0,200,25,40,40)
    MONSTER    (0,225,0)
    DOOR       (0,190,1,190)
    SPAWNER    (0,0, 80, 250, -10)
    SPAWNER    (0,10, 80, 250, -15)
    NEXT
};
ROOM(8,1) {
    HERO       (0,0)
    DOOR       (0,30,0,30)
    SWITCH     (0,40,0b00010000)
    FORCEFIELD (0b00100000, 50, 1)
    BUTTON     (0b00001000,60,0b00111000, 0, 0)
    ITEM       (0,100,2)
    ITEM       (0,110,4)
    DOOR       (0,130,1,200)
    WALL       (0,140,20)
    SWITCH     (0,170,0b00101000)
    FORCEFIELD (0b00010000, 180, 1)
    DOOR       (0,190,0,190)
    SPAWNER    (0,0, 80, 250, -10)
    SPAWNER    (0,10, 80, 250, -15)
    END
};

/**
 * introduction to mines
 * patience is key
 */
ROOM(9,0) {
    HERO       (0,0)
    MINE       (0,40)
    MONSTER    (0,60, -5)
    DOOR       (0,70,1,70)
    MONSTER    (0,80, 5)
    MINE       (0,100)
    MINE       (0,115)
    MONSTER    (0,120, 5)
    MONSTER    (0,140, 5)
    MINE       (0,150)
    DOOR       (0,170,2,170)
    MINE       (0,190)
    MINE       (0,200)
    MINE       (0,225)
    SPAWNER    (0,0, 40, 250, -3)
    SPAWNER    (0,10, 40, 250, -7)
    NEXT
};
ROOM(9,1) {
    HERO       (0,0)
    DOOR       (0,30,1,220)
    DOOR       (0,70,0,70)
    SPAWNER    (0,0,30,99,-10)
    WALL       (0,100,30)
    SPAWNER    (0,0,30,130,10)
    ITEM       (0,140,2)
    WORM       (0,180,10)
    DOOR       (0,220,1,30)
    END
};
ROOM(9,2) {
    HERO       (0,0)
    ITEM       (0,10,4)
    ITEM       (0,20,2)
    MINE       (0,30)
    MONSTER    (0,60,10)
    MONSTER    (0,150,-10)
    DOOR       (0,170,0,170)
    END
};

/**
 * this level has a attack type swap item
 * surrounded by two mines.
 * the monsters need to trigger the mines for the player to take the item
 * which he needs in order to destroy the last mine
 */
ROOM(10,0) {
    HERO       (0,0)
    SPAWNER    (0,30, 40, 0, 3)
    DOOR       (0,30, 1, 30)
    MINE       (0,50)
    ITEM       (0,63,2)
    MINE       (0,75)
    DOOR       (0,110, 1, 110)
    MINE       (0,150)
    WORM       (0,160, -8)
    WORM       (0,175, 8)
    WORM       (0,190, -12)
    DOOR       (0,200, 1, 200)
    NEXT
};
ROOM(10,1) {
    HERO       (0,2)
    DOOR       (0,30, 0, 30)
    DOOR       (0,110, 0, 110)
    FORCEFIELD (0,150,1)
    DOOR       (0,200, 0, 200)
    END
};

/**
 * its time for the first boss battle
 */
ROOM(11,0) {
    // Introduction - BOSS & WALL
    // BOSS - BATTLE
    HERO       (0, 0)
    BUTTON     (0b00010000, 10, 0b00010000, 0, 0)
    MONSTER    (0, 18, 0)
    WALL       (0b00010000, 20, 20)
    MONSTER    (0, 44, 5)
    BOSS       (0,190, 50, 125)
    NEXT
};

/**
 * introduction to portals
 */
ROOM(12,0) {
    // Introduction - PORTAL
    HERO       (0,0)
    PORTAL     (0,100, 200)
    MONSTER    (0,125,0)
    MONSTER    (0,175,0)
    PORTAL2    (0,200, 100)
    DOOR       (0,220,1,220)
    NEXT
};
ROOM(12,1) {
    HERO       (0,0)
    ITEM       (0,10,2)
    LAVA       (0,20,20,0,30)
    ITEM       (0,50,4)
    PORTAL     (0b00010000,100,200)
    PORTAL     (0b00000001,200,100)
    SWITCH     (0,125,0b00010001)
    WALL       (0,140,20)
    SWITCH     (0,175,0b00010001)
    PORTAL2    (0,200, 100)
    DOOR       (0,220,0,220)
    END
};

/**
 * a simple level with the portal
 * and a mine.
 * the player has to go through the portal and
 * disable the wall so the monsters can destroy
 * the mine
 */
ROOM(13,0) {
    HERO       (0,0)
    DOOR       (0,30,1,30)
    PORTAL     (0,50, 140)
    MINE       (0,75)
    MONSTER    (0,80,0)
    WALL       (0b10000000,100, 20)
    SWITCH     (0, 125, 0b10000000)
    PORTAL2    (0,140, 50)
    BUTTON     (0b00010000,160,0b00011000,0,0)
    FORCEFIELD (0b00001000,190,1)
    DOOR       (0,200,1,200)
    SPAWNER    (0,0,40, 250, -15)
    NEXT
};
ROOM(13,1) {
    HERO       (0,0)
    DOOR       (0,30,0,30)
    PORTAL     (0,50,160)
    LAVA       (0,80,40,20,20)
    WORM       (0,130,10)
    FORCEFIELD (0b00010000,190,1)
    DOOR       (0,200,0,200)
    END
};

/**
 * non-inverted portals
 */
ROOM(14,0) {
    HERO       (0,0)
    MONSTER    (0,50,-8)
    MONSTER    (0,60,8)
    PORTAL     (0,180,80)
    DOOR       (0,110,1,110)
    WALL       (0,120,20)
    ITEM       (0,150,1)
    MONSTER    (0,200,-8)
    DOOR       (0,210,1,210)
    FORCEFIELD (0,230,1)
    NEXT
};
ROOM(14,1) {
    HERO       (0,0)
    ITEM       (0,20,2)
    FORCEFIELD (0,40,0)
    DOOR       (0,110,0,110)
    LAVA       (0,120,80,25,25)
    DOOR       (0,210,0,210)
    END
};

/**
 * lava behind fog
 * lava and fog are timed so the monsters come as a surprise quite often.
 */
ROOM(15,0) {
    HERO       (0,0)
    LAVA       (0,50, 25, 30, 40)
    MONSTER    (0,100,0)
    LAVA       (0,125, 25, 0, 40)
    FOG        (0,130, 75, -20)
    MONSTER    (0,170,0)
    LAVA       (0,200, 25, 30, 20)
    SPAWNER    (0, 0, 40, 250, -15)
    NEXT
};

/**
 * two spawners that spawn a lot of monsters
 * can be deaktivated by using the switches, but
 * only one can be deaktivated at a time
 */
ROOM(16,0) {
    HERO       (0,0)
    WATER      (0,60, 50, 15)
    MONSTER    (0,100, 0)
    DOOR       (0,120,1,120)
    MONSTER    (0,150, 0)
    PORTAL     (0,220,170)
    SPAWNER    (0,0,40, 177, -7)
    WALL       (0,180,30)
    FORCEFIELD (0,230,1)
    FOG        (0,210, 25, -10)
    FOG        (0,230, 25, -14)
    NEXT
};
ROOM(16,1) {
    SPAWNER    (0b10000000,0, 30, 0, 15)
    SPAWNER    (0b10000000,10, 30, 0, 10)
    HERO       (0,0)
    DOOR       (0b00000011, 20, 2, 20)
    SWITCH     (0,50,0b10000010)
    WORM       (0,72,20)
    DOOR       (0,120,0,120)
    SWITCH     (0,150,0b01000001)
    ITEM       (0,200,1)
    FOG        (0,210, 25, -10)
    SPAWNER    (0b01000000,0, 30, 250, -15)
    SPAWNER    (0b01000000,10, 30, 250, -10)
    END
};
/**
 * same as the secret room in floor 28.
 * Previously, this was a secret room as well, but I think
 * there should only be one of them.
 * 
 * Instead, this room features hidden monsters and a button
 * under the fog. When getting the extra live, the player
 * will activate the button which "spawns" a new monster
 * in place of the old one.
 */
ROOM(16,2) {
    HERO       (0,0)
    DOOR       (0, 20, 1, 20)
    FOG        (0,40, 30, 0)
    MONSTER    (0b00010000,40,0)
    MONSTER    (0b00001000,40,0)
    FOG        (0,90, 40, 0)
    BUTTON     (0,100,0b00011000,0,0)
    ITEM       (0,150,4)
    WATER      (0,170,40,-10)
    WORM       (0,230,8)
    // SECRET
    END
};

/**
 * second labyrinth
 * or more of a "enter passcode" kind of thing.
 * 
 * if you are wondering: the code is 2-1-3
 */
ROOM(17,0) {
    HERO       (0,0)
    DOOR       (0,50,0,50)
    LAVA       (0,60,30,20,20)
    DOOR       (0,100,4,100)
    LAVA       (0,110,30,20,20)
    DOOR       (0,150,2,150)
    END
};
ROOM(17,1) {
    HERO       (0,0)
    SPAWNER    (0,0,30,0,10)
    DOOR       (0,50,0,50)
    WATER      (0,60,30,-10)
    DOOR       (0,100,4,100)
    WATER      (0,110,30,10)
    DOOR       (0,150,2,150)
    END
};
ROOM(17,2) {
    HERO       (0,0)
    SPAWNER    (0,0,30,0,10)
    DOOR       (0,50,0,50)
    PORTAL     (0,70,130)
    DOOR       (0,100,4,100)
    DOOR       (0,150,2,150)
    END
};
ROOM(17,3) {
    HERO       (0,0)
    SPAWNER    (0,0,30,0,10)
    DOOR       (0,50,0,50)
    LAVA       (0,60,30,20,20)
    DOOR       (0,100,4,100)
    LAVA       (0,110,30,20,20)
    DOOR       (0,150,5,150)
    MONSTER    (0,235,0)
    MONSTER    (0,240,0)
    END
};
ROOM(17,4) {
    HERO       (0,0)
    SPAWNER    (0,0,30,0,10)
    DOOR       (0,50,3,50)
    WATER      (0,60,30,-10)
    DOOR       (0,100,4,100)
    WATER      (0,110,30,10)
    DOOR       (0,150,2,150)
    MONSTER    (0,240,0)
    END
};
ROOM(17,5) {
    HERO       (0,0)
    SPAWNER    (0,0,30,0,10)
    DOOR       (0,50,0,50)
    PORTAL     (0,70,130)
    DOOR       (0,100,4,100)
    DOOR       (0,150,2,150)
    MONSTER    (0,235,0)
    MONSTER    (0,240,0)
    MONSTER    (0,245,0)
    NEXT
};

// Boss level:
// water#portal..boss2 ... boss1#portal..boss3
ROOM(18,0) {
    HERO       (0,0)
    WATER      (0, 30, 40, 20)
    PORTAL     (0, 70, 190)
    BOSS       (0, 174, 110, 220)
    PORTAL2    (0, 190, 70)
    NEXT
};

// Portal level:
// switches + portals to get to the end
ROOM(19,0) {
    HERO       (0,0)
    SWITCH     (0, 10, 0b10000001)
    PORTAL     (0b10000000, 20, 220)
    PORTAL     (0b00000001, 20, 80)
    PORTAL2    (0b00000001, 80, 20)
    PORTAL     (0b00000001, 150, 220)
    SWITCH     (0, 130, 0b10000001)
    FORCEFIELD (0, 180, 1)
    SWITCH     (0, 200, 0b10000001)
    PORTAL2    (0b10000000, 220, 20)
    PORTAL2    (0b00000001, 220, 150)
    NEXT
};

/**
 * a button trap:
 * the player has to step off the button and wait until the trap is open again.
 * However, the exit is only open for a short amount of time after the trap reopens.
 */
ROOM(20,0) {
    RESET      (0b00000001)
    HERO       (0, 0)
    MONSTER    (0, 40, 0)
    MONSTER    (0, 40, -4)
    MONSTER    (0, 40, 8)
    FORCEFIELD (0b00000001, 80, 1)
    BUTTON     (0, 100, 0b00010010, 0, 40)
    BUTTON     (0, 100, 0b00000001, 0, 10)
    FORCEFIELD (0b00000001, 115, 1)
    LAVA       (0, 140, 20, 10, 30)
    LAVA       (0, 160, 20, 20, 30)
    FORCEFIELD (0b00010000, 200, 1)
    MONSTER    (0, 230, 10)
    NEXT
};

// Lava & Water level:
// switches under lava;
// switches to turn current, so one can go through portal with sword on
// to kill monsters at the other side
ROOM(21,0) {
    HERO       (0,0)
    LAVA       (0, 50, 30, 0, 25)
    SWITCH     (0, 65, 0b01000000)
    FORCEFIELD (0b01000000, 90, 1)

    SWITCH     (0, 100, 0b10000001)
    WATER      (0b10000000, 130, 35, -20)
    WATER      (0b00000001, 130, 35, 20)
    PORTAL     (0, 160, 230)
    WORM       (0, 216, 0)
    PORTAL2    (0, 230, 160)
    NEXT
};

/**
 * Multi-room Bossfight:
 * The hero has to fight the same boss multiple times, because the
 * main room has to be changed from the second room in order to become
 * fully solvable.
 * Also has a door in the second room that will instakill the hero whenever
 * entering it (because the boss is on top of that door in the main room)
 */
ROOM(22,0) {
    HERO       (0, 0)
    ITEM       (0, 60, 0)
    FORCEFIELD (0, 80, 0)
    BOSS       (0, 100, 200, 50)
    DOOR       (0, 103, 1, 103)
    FORCEFIELD (0b00010000, 150, 1)
    FORCEFIELD (0b00000001, 150, 0)
    ITEM       (0b00010000, 170, 1)
    ITEM       (0b00000001, 170, 0)
    DOOR       (0, 203, 1, 203)
    NEXT
};
ROOM(22,1) {
    HERO       (0, 0)
    ITEM       (0, 60, 0)
    FORCEFIELD (0, 80, 1)
    DOOR       (0, 103, 0, 103)
    LAVA       (0, 110, 20, 20, 30)
    DOOR       (0b1, 140, 0, 140)
    FORCEFIELD (0b00010000, 150, 0)
    FORCEFIELD (0b00000001, 150, 1)
    DOOR       (0, 203, 0, 203)
    LAVA       (0, 220, 20, 10, 30)
    FORCEFIELD (0, 230, 1)
    SWITCH     (0, 240, 0b00010001)
    END
};

/**
 * second full assault
 * 
 */
ROOM(23,0) {
    SPAWNER    (0,0,30,0,10)
    HERO       (0,0)
    LAVA       (0,40,20,20,30)
    WORM       (0,20,8)
    MONSTER    (0,80,-8)
    PORTAL     (0,160,80)
    ITEM       (0,100,2)
    DOOR       (0,120,0,0)
    ITEM       (0,140,4)
    MONSTER    (0,150,-8)
    MONSTER    (0,150,-10)
    MONSTER    (0,170,8)
    MONSTER    (0,170,10)
    WORM       (0,190,-10)
    WORM       (0,210,10)
    DOOR       (0,200,0,120)
    SPAWNER    (0,10,40,250,-10)
    SPAWNER    (0,20,40,250,-20)
    NEXT
};

/**
 * third labyrinth
 * 
 */
ROOM(24,0) {
    HERO       (0,0)
    LAVA       (0,20,20,20,30)
    DOOR       (0,50,1,50)
    FORCEFIELD (0b00010000, 90, 1)
    SWITCH     (0, 100, 0b00010001)
    FORCEFIELD (0b00000001,110, 0)
    WORM       (0,130,-10)
    DOOR       (0,150,1,150)
    DOOR       (0,180,2,180)
    DOOR       (0b00000010,210,1,210)
    SPAWNER    (0,0,40,250,-10)
    END
};
ROOM(24,1) {
    HERO       (0,0)
    SPAWNER    (0,0,40,0,10)
    ITEM       (0,40,0)
    DOOR       (0,50,0,50)
    ITEM       (0,60,1)

    WALL       (0,70,20)

    DOOR       (0,100,2,100)
    MINE       (0,110)
    FORCEFIELD (0,140,1)
    DOOR       (0,150,0,150)
    FORCEFIELD (0,160,1)
    SWITCH     (0,170,0b00100010)

    WALL       (0b00000010,180,20)

    DOOR       (0b00000010,210,0,210)
    FORCEFIELD (0b00000010,230, 0)
    ITEM       (0b00000010,240,2)
    SPAWNER    (0b00000010,0,40,250,5)
    END
};
ROOM(24,2) {
    HERO       (0,0)
    SPAWNER    (0,0,40,0,10)
    DOOR       (0,100,1,100)
    PORTAL     (0,220,120)
    DOOR       (0,180,0,180)
    SPAWNER    (0,0,40,250,-10)
    NEXT
};

/**
 * previously the final battle (but not anymore)
 * two bosses + 2 spawner should be hard enough
 */
ROOM(25, 0) {
    // BOSS - BATTLE
    HERO       (0,0)
    BOSS       (0,75,175,50)
    LAVA       (0,110, 30, 0, 40)
    BOSS       (0,50,100,200)
    SPAWNER    (0, 0   , 80, 250, -3)
    SPAWNER    (0, 10, 80, 250, -15)
    NEXT
};

/**
 * each switch toggles two of the forcefields.
 * the player has to find the correct combination.
 * 
 */
ROOM(26,0) {
    HERO       (0, 0)
    MONSTER    (0, 20, 0)
    SWITCH     (0, 30, 0b00100101)
    SWITCH     (0, 40, 0b00101010)
    SWITCH     (0, 50, 0b01010010)
    BUTTON     (0, 60, 0b10010001, 0, 10)
    FORCEFIELD (0b00000001, 70, 1)
    FORCEFIELD (0b00000010, 80, 1)
    FORCEFIELD (0b00010000, 90, 1)
    FORCEFIELD (0b00100000,100, 1)
    MONSTER    (0, 108, 0)
    LAVA       (0b10000000,120, 20, 0, 40)
    LAVA       (0b01000000,140, 20, 10, 40)
    LAVA       (0b00001000,160, 20, 20, 40)
    LAVA       (0b00000100,180, 20, 30, 40)
    SPAWNER    (0, 50, 40, 250, -8)
    SPAWNER    (0, 40, 40, 250, -8)
    NEXT
};

/**
 * this level relies on the timing of the first button that exists in every sublevel
 * activating the button allows to progress through the level
 */
ROOM(27,0) {
    HERO       (0, 0)
    BUTTON     (0, 10, 0b00010001, 0, 20)
    PORTAL     (0b00010000, 20, 180)
    PORTAL     (0b00000001, 20, 100)
    DOOR       (0, 40, 1, 40)
    WALL       (0, 50, 10)
    DOOR       (0, 70, 1, 70)
    PORTAL     (0b00000001, 150, 180)
    FORCEFIELD (0, 200, 1)
    DOOR       (0, 220, 2, 220)
    FORCEFIELD (0, 240, 0)
    NEXT
};
ROOM(27,1) {
    HERO       (0, 0)
    SPAWNER    (0, 0, 40, 0, 8)
    ITEM       (0, 20, 1)
    DOOR       (0, 40, 0, 40)
    WALL       (0, 50, 10)
    DOOR       (0, 70, 0, 70)
    END
};
ROOM(27,2) {
    HERO       (0, 0)
    MONSTER    (0, 0, 0)
    ITEM       (0, 10, 0)
    ITEM       (0, 20, 4)
    LAVA       (0, 80, 30, 0, 15)
    LAVA       (0, 140, 30, 0, 15)
    DOOR       (0, 220, 0, 220)
    SPAWNER    (0, 0, 40, 250, -8)
    SPAWNER    (0, 0, 40, 250, -12)
    END
};

/**
 * setup for the secret room
 * you have to be fast enough
 */
ROOM(28,0) {
    RESET      (0b00100011)
    HERO       (0, 0)
    BUTTON     (0, 12, 0b00000001, 0, 100)
    BUTTON     (0, 12, 0b00100010, 0, 60)
    FORCEFIELD (0b00100000, 20, 1)
    LAVA       (0, 30, 5, 0, 15)
    LAVA       (0, 35, 5, 5, 15)
    LAVA       (0, 40, 5, 10, 15)
    LAVA       (0, 45, 5, 15, 15)
    PORTAL     (0, 80, 180)
    WATER      (0, 90, 20, -8)
    LAVA       (0, 115, 5, 15, 15)
    LAVA       (0, 120, 5, 10, 15)
    LAVA       (0, 125, 5, 5, 15)
    LAVA       (0, 130, 5, 0, 15)
    BUTTON     (0, 150, 0b00100010, 0, 60)
    ITEM       (0b00000010, 160, 4)
    FORCEFIELD (0b00100000, 160, 1)
    PORTAL2    (0, 80, 180)
    FORCEFIELD (0b00100000, 210, 1)
    DOOR       (0b00000001, 230, 1, 230)
    SPAWNER    (0, 0, 30, 250, -12)
    NEXT
};
/**
 * a secret room for those who are really into this game <3
 */
ROOM(28,1) {
    HERO       (0,0)
    ITEM       (0,10,4)
    MONSTER    (0,100,-8)
    MONSTER    (0,100,8)
    WORM       (0,20,8)
    WORM       (0,40,12)
    FOG        (0b00000001,40, 30, 0)
    FOG        (0b00000001,90, 40, 0)
    WATER      (0,170,40,-10)
    DOOR       (0, 230, 0, 230)
    SECRET
    END
};

/**
 * the final floor and the final boss fight.
 * 
 * the player has to die once inside the portals to solve the puzzle.
 */
ROOM(29,0) {
    HERO       (0,0)
    DOOR       (0,10,1,10)
    BOSS       (0,220,110,160)
    PORTAL     (0b00010000,190,70)
    PORTAL     (0b00000001,70,190)
    SWITCH     (0,80,0b00010001)
    FORCEFIELD (0,90,0)
    WALL       (0,130,20)
    SWITCH     (0,160,0b00010001)
    DOOR       (0,220,1,220)
    LAVA       (0,230,20,0,30)
    NEXT
};
ROOM(29,1) {
    HERO       (0,0)
    FORCEFIELD (0b00100000,20,0)
    FORCEFIELD (0b00000010,20,1)
    ITEM       (0b00100000,30,0)
    ITEM       (0b00000010,30,1)
    SWITCH     (0,50,0b00110011)
    PORTAL     (0b00010000,180,70)
    MONSTER    (0,90,0)
    LAVA       (0,100,20,0,40)
    PORTAL     (0b00000001,70,180)
    DOOR       (0,10,0,10)
    BOSS       (0,25,220,150)
    DOOR       (0,220,0,220)
    WALL       (0,240,10)
    END
};

/**
 * this room can be used to check the color correction.
 * contains every color and entity in the game.
 */
ROOM(255,0) {
    HERO       (0, 0)
    PORTAL     (0, 10, 50)
    WATER      (0, 20, 25, 10)
    ITEM       (0, 60, 2)
    ITEM       (0, 70, 4)
    BUTTON     (0, 80, 0, 0, 10)
    SWITCH     (0, 90, 0)
    DOOR       (0, 100, 0, 100)
    FOG        (0, 110, 10, 0)
    ITEM       (0, 120, 1)
    FORCEFIELD (0, 130, 1)
    ITEM       (0, 140, 1)
    ITEM       (0, 150, 0)
    FORCEFIELD (0, 160, 0)
    MINE       (0, 170)
    MONSTER    (0, 180, 0)
    WALL       (0, 190, 10)
    WORM       (0, 200, 5)
    LAVA       (0, 220, 10, 20, 20)
    BOSS       (0, 235, 235, 235)
    NEXT
};
#endif

// # ================================================================ #
// # ::  LOAD                                                      :: #
// # ================================================================ #

/**
 * the idle floor "screen saver"
 */
ROOM(255,255) {
    MONSTER    (0, 0, -5)
    BOSS       (0, 48, 48, 48)
    WORM       (0, 70, 3)
    MONSTER    (0, 120, -5)
    MONSTER    (0, 215, -5)
    WORM       (0, 220, -2)
    MONSTER    (0, 246, -6)
    END
};

#undef SECRET
#define MIX(f,r) (((f) << 3) | (r))
#define CASE(f,r) case (((f) << 3) | (r)): ptr = F ## f ## R ## r; break;
// replace with the following two lines, if there is are more than 32 floors or a floor with more than 8 rooms.
//#define MIX(f,r) ((((uint16_t) f) << 8) | (uint16_t) r)
//#define CASE(f,r) case ((((uint16_t) (f)) << 8) | (uint16_t) (r)): ptr = F ## f ## R ## r; break;
bool InternalLoader::load(uint8_t  floor, uint8_t room) {
    const uint8_t* ptr;
    switch (MIX(floor, room)) {
        #if ENABLE_INTERNAL_DUNGEON
        CASE(0,0);
        CASE(1,0);
        CASE(2,0);
        CASE(2,1);
        CASE(3,0);
        CASE(4,0);
        CASE(4,1);
        CASE(5,0);
        CASE(5,1);
        CASE(6,0);
        CASE(6,1);
        CASE(6,2);
        CASE(7,0);
        CASE(7,1);
        CASE(7,2);
        CASE(7,3);
        CASE(7,4);
        CASE(8,0);
        CASE(8,1);
        CASE(9,0);
        CASE(9,1);
        CASE(9,2);
        CASE(10,0);
        CASE(10,1);
        CASE(11,0);
        CASE(12,0);
        CASE(12,1);
        CASE(13,0);
        CASE(13,1);
        CASE(14,0);
        CASE(14,1);
        CASE(15,0);
        CASE(16,0);
        CASE(16,1);
        CASE(16,2);
        CASE(17,0);
        CASE(17,1);
        CASE(17,2);
        CASE(17,3);
        CASE(17,4);
        CASE(17,5);
        CASE(18,0);
        CASE(19,0);
        CASE(20,0);
        CASE(21,0);
        CASE(22,0);
        CASE(22,1);
        CASE(23,0);
        CASE(24,0);
        CASE(24,1);
        CASE(24,2);
        CASE(25,0);
        #if !SHORTEN_INTERNAL_DUNGEON
        // these rooms are not included in the small version
        CASE(26,0);
        CASE(27,0);
        CASE(27,1);
        CASE(27,2);
        CASE(28,0);
        CASE(28,1);
        CASE(29,0);
        CASE(29,1);
        CASE(255,0);
        #endif
        #endif
        CASE(255,255);
        default: return false;
    }

    while (true) {
        uint8_t type = pgm_read_byte_near(ptr++);
        LOG("type: ");
        LOG_LN(type);
        switch (type) {
            case RESET_ID: Entity::resetMaskPartially(pgm_read_byte_near(ptr++)); continue;
            case SECRET_ID: Sound::play(Sound::SECRET); continue;
            case NEXT_ID: Hero::setGoal(true); // no break;
            case END_ID: return true;
        }

        uint8_t arg_count = Entity::getArgCount(type);
        uint8_t args[LEVEL_DESCRIPTION_MAX_PARAMETER_PER_UNIT];
        for (uint8_t arg = 0; arg < arg_count; arg++) {
            args[arg] = pgm_read_byte_near(ptr++);
        }

        Entity::spawn(type, &args[0]);
    }
}
