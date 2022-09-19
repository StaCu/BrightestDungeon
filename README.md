# Brightest Dungeon

A 1D dungeon arcade&puzzle game inspired by a game I saw at 35C3.

BrightestDungeon is rather simple: You are the green dot and can move on a one dimensional led strip, but beware the red dots!
Fend for yourself against the evil monsters and progress through the different levels of the dungeon.
But keep in mind that BrightestDungeon is, at heart, a puzzle game.
Finishing a floor is not as simple as killing all the enemies within it.
You will have to prove your wits and worthiness by solving the puzzle on each floor.

## Getting it up and running

The minimal hardware requirements to play BrightestDungeon include a microcontroller (Atmega328p), LED strip (WS2812), potentiometer and a button.
An accelerometer (MPU6050), sound driver and sdcard reader can improve the experience.
The code can be easily ported to other platforms as well.
Alternatively, one can also be play from the linux terminal.

### Microcontroller

Each supported platform has a file platform/\<platform\>/Platform.md , which contains information about which components are
required, how to wire them as well as information about how to en-/disable some of them.

To program the microcontroller with Arduino IDE, open BrighestDungeon.ino and copy the required libraries.
To upload the code with PlatformIO, use one of the following lines:

```
pio run -e <environment> --target upload
pio run -e <environment> --target upload && pio device monitor
```

### Simulator

To run the simulator and play the game inside the terminal, run:

```
pio run -e simulator && .pio/build/simulator/program
```

The ncurses library is a prerequisite.
It is preinstalled on most linux distibutions, or can be installed using:

```
sudo apt install libncurses-dev
```

You can play using A (left), D (right), S (attack), E (options), Q (quit).
However, this meant to be more for debugging and creating new levels rather than actually playing BrightestDungeon.

## Project Structure

The following is a simplified structure of the project to show where to find information about its different parts.

folder/file                  | content
-----------------------------|-------------
cad/                         |
\|-- img/                    | some images to look at
\|-- Build Instructions.md   | how to build the game
dng/                         |
\|-- 0-f/                    | sdcard-loadable dungeons
\|-- Create Dungeon.md       | how to create new dungeons/floors/rooms
\|-- Entity Cheat Sheat.md   | explanation of each entity
\|-- Runtime Settings.md     | explanation of runtime settings
\|-- template.txt            | template for a dungeon room
platform/                    |
\|-- lib/                    | common libraries
\|-- \<platforms\>/          | 
\|-- \|-- lib/               | libraries for each platform
\|-- \|-- Platform.md        | information about the platform
\|-- Porting.md              | how to port to a new platform
src/                         | 
BrightestDungeon.ino         | entry point for ArduinoIDE
LICENSE                      | GPLv3
platformio.ini               | build tool instructions
README.md                    | this file
sdcard.sh                    | copy everything to an sdcard
