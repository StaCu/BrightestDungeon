# Simulator

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

## Required Libraries:

- Common
- Specific
  - ncurses
  - Platform
