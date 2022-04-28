# Creating new Dungeons, Floors and Rooms

First up, in Settings.h ENABLE_FILE_LOADER must be defined as true.

## Background

Brightest Dungeon loads a room by using the following path scheme:
/dng/X/YYY/ZZZ.txt
X: dungeon number [0-9]
YYY: floor number [0-255]
ZZZ: room number [0-255]

If the file does not exist, it will be treated like the player having beaten the entire dungeon (i.e. showing the victory animation).
If the file does exist, loading it will be successful, no matter the content.
Anything that does not match the correct syntax will be ignored or interpreted as 0.

## Creating a dungeon

A dungeon simply consists of multiple floors, starting with floors 000.
After finishing a floors, the next one is loaded.
If no room 000 exists within the next floor, the player has finished the dungeon.

## Creating a floor

A floor consists of one or more rooms, with one room being the 000 (the first room of the floor).
Doors can be used to switch between the different rooms.
Due to limited memory, (re-)loading a room will restore every entity that got destroyed previously (this actually makes for an interesting puzzle mechanic).

## Creating a room

A room is 250 units long and updated 50 times per second (every UPDATE_TIME ms).
Each unit consists of 32 sub steps.
 => speed of 10 results in 10 * 50 / 32 = 15.625 steps/second
(can be changed in src/Settings.h)
I find that a speed of 3 to 20 is reasonable for monsters and water current.

The text file describing a room is read line by line.

### Entities

Each line contains one entity and its parameters.

 name       | parameters               | length | hint
------------|--------------------------|--------|-----
 NEXT       |                          |        | there is a goal at the end to progress to the next floor [^0]
 END        |                          |        | there is no goal at the end to progress to the next floor (optional) [^0]
 RESET      | mask                     |        | resets the specified bits of the global mask
------------|--------------------------|--------|-----
 HERO       | mask pos                 |    1   | must be a part of the level (otherwise it's a boring level...)
            |                          |        | ofter death the previous spawn position is used (usually that is what you want)
 ITEM       | mask pos type            |    1   |
------------|--------------------------|--------|-----
 MONSTER    | mask pos speed           |    1   |
 BOSS       | mask pos pos pos         |   16   | a boss respawns 2 times
 WORM       | mask pos speed           |   16   |
 SPAWNER    | mask time time pos speed |    0   | the 1st time is the first activation,
            |                          |        | the 2nd the interval for every
            |                          |        | recurring activation
------------|--------------------------|--------|-----
 BUTTON     | mask pos op time time    |    6   |
 DOOR       | mask pos room pos        |    6   | the 2nd position is the destination
 FOG        | mask pos size speed      | size   |
 FORCEFIELD | mask pos color           |    6   |
 LAVA       | mask pos size time time  | size   | the 1st time is the first activation,
            |                          |        | the 2nd the interval for every
            |                          |        | recurring activation
 MINE       | mask pos                 |    1   |
 PORTAL     | mask pos pos             |    6   | the second position is the second portal
 SWITCH     | mask pos op              |    6   |
 WALL       | mask pos size            | size   |
 WATER      | mask pos size speed      | size   |
------------|--------------------------|--------|-----

[^0] anything after this will be ignored

### Parameters

 name  | range        | description
-------|--------------|-------------
 pos   | [0, 250]     |
 size  | [0, 250]     |
 speed | [-128, 127]  | note: the max player speed is slightly below 100
 time  | [0, 255]     | multiply by 100ms, 0 means no repetition
 room  | [0, 255]     | 
 type  | [1, 7]       | 0: set color to green (0), (default)
       |              | 1: set color to blue (1),
       |              | 2: swap sword <=> shot, (default: sword)
       |              | 3: invert control direction, (default: not inverted)
       |              | 4: extra life,
 color | [0, 1]       | 0: color green (0)
       |              | 1: color blue (1)
 op    | [0, 255]     |
 mask  | [0, 255]     |
-------|--------------|-------------

Note: use binary notation by prefixing the number with a b or 0b (128 or 0b10000000).

### MASKING

Masks are a big part of the puzzle game. 
Each entity has a mask and there is a global mask.
if (entity_mask & global_mask == entity_mask), the entity is active, otherwise not.
This means not drawn, no update, no collision.

Switches and Buttons can change the global mask with the op parameter.
new_global_mask = global_mask ^ op;

When loading a new floor, the global mask is reset to 0b11110000 (this also means that it persists accross rooms and deaths on the same floor).
An entity with mask 0b10000000 will be active until the msb is flipped.
An entity with mask 0b00000001 will be inactive until the lsb is flipped.
An entity with mask 0b00000000 will always be active.

### COMMENTS

There are no comments.
If the first word does not match any of the keywords listed above, the line will be ignored.
Anything behind a complete command is also ignored.
A line has a maximum of FILEIO_BUFFER_SIZE (64) characters, anything beyond that is ignored.
If a number is missing or contains other symbols, it is interpreted as a 0.
 => there are no errors apart from "file not found".

BLABLA      this line will be treated as a "comment", because BLABLA is not a keyword
HERO as     this line will do the same as HERO 0 0, as does the next one
HERO                                                               1 2  (more than 64 chars)
HERO(0,0)   '(' ',' and '\t' are treated the same as ' '

The maximum amount of entities in the same room is POOL_SIZE, two of which are reserved for attacks.
Loading more entities will not do anything.
Keep in mind that a PORTAL comprises two entities.

### EXAMPLE

++ MAX LINE LENGTH +++++++++++++++++++++++++++++++++++++++++++++
HERO(0, 0)                # spawns a hero at position 0
LAVA(0, 50, 25, 0, 20)    # creates a lava pool at position 50
                          # with length 25. it activates after
                          # 0 seconds and switches between
                          # active/inactive every 20*100ms = 2sec
MONSTER(0, 200, 0)        # this monster will always show up
MONSTER(1, 210, 10)       # this monster is masked
NEXT                      # end of level
                          # once the hero reaches the goal, the
                          # next floor will be loaded
++ MAX LINE LENGTH +++++++++++++++++++++++++++++++++++++++++++++
