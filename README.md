# Brightest Dungeon

A 1D dungeon arcade&puzzle game inspired by a game I saw at 35C3.

Brightest Dungeon is rather simple: You are the green dot and can move on a one dimensional led strip, but beware the red dots!
Fend for yourself and progress through the different levels of the dungeon.
But keep in mind that Brightest Dungeon is, at heart, a puzzle game.
Finishing a floor is not as simple as killing all the enemies within it.
You will have to prove your wits and worthiness by solving the puzzle each floor represents.

## Entity Cheat Sheet

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

## Getting it up and running

Here is a list of the different hardware elements required to run the game.

### Atmega328P (16MHz)

The game was originally developed for the Atmega328P, using it will not require any changes to the code base and all features can be used.
The Atmega328P must be clocked at 16MHz, because the LEDs used (WS2811) use a protocol depending on the correct timing.
The code to transmit the LED data was written so it works for 16MHz.

### Atmega168P (16MHz)

The game works on the smaller version of the Atmega328P, the Atmega168P, as well.
However, some settings must be disabled/reduced, because the Atmega168P doesn't have enough sram and progmem required for those features.
In src/Settings.h set ...
```
//#define USE_MPU6050_CONTROLLER
#define LED_STRIP_COUNT 300
#define LED_STATUS_COUNT 0
#define POOL_SIZE 40
#define ENABLE_SOUND false
#define SHORTEN_INTERNAL_DUNGEON true
#define ENABLE_FILE_LOADER false
#define USE_EEPROM false
#define ENABLE_LOGGING false
```
... and the program code should turn out to fit onto the Atmega168P together with a small bootloader.

### WS2812B

To save SRAM (and PROGMEM), the LED control code has been written in Assembler and is only compatible with WS2811 LEDs (and WS2812).
While the code is capable of addressing up to 2^16 LEDs, the MCU reaches its limit at around 500 of them, beyond of which it is not capable of delivering 50 refreshes per second.
Increasing UPDATE_TIME in src/Settings.h will allow for more LEDs.
By default, the minimum number of LEDs is 100, as the level layout suffers from too few pixels.
Both, LED_MIN_COUNT and LED_STRIP_COUNT can be changed in src/Settings.h .
Some LED strips are terrible with regards to color correctness. 
```
#define COLOR_PALETTE 1
```
switches to a second color palette that ensures the different entities are still distinguishable (but the game will look worse).

### Simple Controller

The simple controller consists of a button for attacking and a potentiometer for moving.

### MPU6050

The better controller consists of a button for attacking and the MPU6050 for moving.

### Passive Buzzer

Used to generate the sound.

### SDCard Adapter and SDCard

To allow for new dungeons without the need to recompile, BrightestDungeon is capable of loading levels from an SDCard.

### Button

One button for entering the in game options menu.

### Pins

Most of the pins have been hardcoded (but the assembler code can of course be changed).
The default values are:
Usage              | Pin
-------------------|---------------
Simple Controller  |
 \- Potentiometer  | A5
 \- Attack Button  | D2
MPU6050            |
 \- SCL            | A5
 \- SDA            | A4
 \- Attack Button  | D2
LED                |
 \- Data           | D8
Sound              |
 \- Passive Buzzer | D9, D10
Options            | D3
SD Card            | (many sdcard modules use 3.3V)
 \- MOSI           | D11
 \- MISO           | D12
 \- CLK            | D13
 \- CS             | D4

Connecting Ground and Vcc is all that is left.

## Setup

The compile time settings are all listed and explained within src/Settings.h .

### Microcontroller

To program the microcontroller with Arduino IDE, open BrighestDungeon.ino with it.
To upload the code with PlatformIO, use one of the following lines:

```
pio run --target upload
pio run --target upload && pio device monitor
```

### Simulator

The simulator can be started with a directory from which to load external dungeons and store the eeprom simulation and logging information.
To run the simulator and play the game inside the terminal, run:

```
mkdir build
cd build/
cmake .. 
make dungeon
./dungeon ../
```

The ncurses library is a prerequisite (preinstalled on most linux distibutions).
You can play using A (left), D (right), S (attack), E (options), Q (quit).
However, this meant to be more for debugging and creating new levels rather than actually playing BrightestDungeon.

### Secret Code

To generate a secret morse code that can be integrated into the game write the text to secret/main.cpp and run:

```
mkdir build
cd build/
cmake .. 
make secret
./secret
mv Secret.h ../src/Secret.h
```

###

To copy everything to an sdcard, run:

```
./sdcard.sh <path>
```

## Creating new Dungeons, Floors and Rooms

See /dng/README on how to write and load your own dungeons.

## In Game Settings

The runtime settings are listed within src/level/Options.cpp.
While they can be changed at runtime, their default values can be set in src/Settings.h.
When using the EEPROM feature to remember the settings, however, those default values become less important.

The options menu can be opened by pressing the OPTIONS_PIN.
This will show a level with multiple white, gray and green dashes.
Placing the hero on one of them and attacking will change the settings (the green dashes are only spacers and do nothing).

Options layout (WW: white, WG: white/gray, GG: green):
x    | description
-----|---------------------------------
WW   | callibrate the controller
WG   | invert the controller direction
GG   | ---
WW   | led_count --
WW   | led_count ++
WW   | brightness --
WW   | brightness ++
WW   | volume --
WW   | volume ++
GG   | ---
WG   | enable/disable checkpoints
GG   | ---
WG   | dungeon0 [^1]
WG   | dungeon1 [^1]
WG   | dungeon2 [^1]
WG   | dungeon3 [^1]
GG   | ---
WG   | floor0 [^2]
WG   | floor1 [^2]
WG   | floor2 [^2]
WG   | floor3 [^2]
WG   | floor4 [^2]
WG   | floor5 [^2]
WG   | floor6 [^2]
WG   | floor7 [^2]
GG   | ---
WW   | reload options from eeprom
WW   | write changes to eeprom
GG   | ---
Goal | exit options (keep changes until the next restart)

[^1]: select one of 16 dungeons (0: internal, 1-15: sdcard);
      when opening the options, this will show the currently selected dungeon
[^2]: select a floor from this dungeon and start from there (skip the previous floors);
      when checkpoints are disabled, this floor serves as a start point after loosing all lives;
      when opening the options, this will show the currently selected floor
