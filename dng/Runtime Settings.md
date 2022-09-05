# Runtime Settings

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
