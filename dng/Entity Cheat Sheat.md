# Entity Cheat Sheet

List of all the entities, puzzle pieces and stuff to watch out for in the dungeon.

Entity               | Visual              | Description
---------------------|---------------------|---------------------
Lives                | green/blue/red dots | the first few leds show how many lives you have left (not so important when using checkpoints)
Hero                 | green/blue dot      | you
 \- Goal             | green/blue          | your goal (not every room has one, always at the end)
 \- Sword            | cyan line           | your default attack
 \- Shot             | cyan dot            | your special attack (lost on death)
Item                 |                     |     
 \- sword/shot       |                     | swap attack type
 \- set color green  |                     | change your color to green
 \- set color blue   |                     | change your color to blue
 \- invert control   |                     | invert control
 \- extra life       |                     | +1 live (capped at MAX_LIVES)
---------------------|---------------------|---------------------
Monster              | red dot             | move back and forth, (do not touch!)
 \- Spawner          | none                | spawns new monsters
Worm                 | moving red line     | a bit smarter than normal monsters
Boss                 | red line            | has multiple lives and spawns monsters
---------------------|---------------------|---------------------
Button               | white/gray line     | [^0] with a timed behaviour
Door                 | white line          | enter another room on the same floor
Fog                  | pink line           | hides anything underneath it
ForceField           | blue/green flicker  | you can only pass through if you have matching colors
Lava                 | orange flicker      | save to cross when it does not flicker
Mine                 | yellow dot          | explodes (do not touch!)
Portal               | orange/blue line    | the cake is a lie!
Switch               | white/gray line     | [^0] with a back and forth behaviour
Wall                 | white line          | blockade
Water                | moving blue dots    | the current drags you along with it

[^0]: Brightest Dungeon employs a masking system to change parts of the active room.
      Loading a room loads EVERY entity within it.
      However, each entity has a mask that determines whether it is visible, updated and part of collisions.
      Moving over a button or pressing a switch changes which entities are masked (though some are always there).
