#pragma once

#include <stdint.h>

/**
 * The Floor class handles the loading transitions between different floors and rooms.
 * The game logic is delegated to the Room class.
 * The different load methods are used to initiate a load of a different floor/room.
 * Only one such transition can be active at a time.
 */
class Floor {

private:
    /**
     * Used to differentiate between the different loading modes
     */
    enum State : uint8_t {
        PLAY,

        LOAD_SUCCESS,
        LOAD_FAILURE,
        LOAD_CHECKPOINT,
        LOAD_IDLE,
        LOAD_OPTIONS,
        LOAD_DOOR,

        START_TIMER,
    };

    /**
     * current state
     * (basically it is either in play or one of the different loading modes)
     */
    static State state;

    /**
     * this function resets everything for when the player dies
     */
    static void resetAll();

public:
    /**
     * the current floor index.
     * This variable should not be written to by any other class.
     * If the game is currently idle or in options, the value is that of the last floor visited.
     * After using any of the load functions, this variable may hold the index of the NEXT floor until it has been loaded.
     */
    static uint8_t idx;
    /**
     * when the player enters a door, this variable holds the index
     * of the next room until it has been loaded
     */
    static uint8_t room_idx;

    /**
     * updates the game, including:
     * - everything happening in the current room
     * - animations (explosion, victory)
     * - level loading transition
     */
    static void update();

    /**
     * draws the game, including:
     * - lives & goal
     * - room (and special rooms like options and idle)
     * - animations (explosion, victory)
     */
    static void draw();

private:
    /**
     * actual loading of the next level, including
     * - reset of necessary variables
     * - animations
     * - loading
     */
    static void load();

    /**
     * starts the victory animation
     * this method is called when the game tried to load a floor or room that does not exist 
     * (exception: floor 0, room 0 or when the game just exited the options
     *  the idea here is to not show a victory, when the player didn't beat even a single room).
     */
    static void loadVictory();

public:

    /**
     * initiates loading the options room
     */
    static void loadOptions();

    /**
     * initiates loading the idle room
     */
    static void loadIdle();

    /**
     * loads the floor with the current idx
     * no explosion animation, but implosion, if the was an explosion without implosion before
     * => sounds weird, but basically:
     *    - when the game starts, this is called to load the first level (no implosion)
     *    - when the game was idle, this is called to load the most recent checkpoint 
     *      (implosion, because loading the idle level includes the explosion, but no implosion animation; 
     *       i.e. the ExplosionAnimation is still in the exploded state)
     */
    static void loadImmediately();
    
    /**
     * initiates loading a room of the current floor.
     * No animations, this happens in the next current update (if called during the normal update), otherwise next update.
     * 
     * @param room 
     *      the room to load
     */
    static void loadDoor(uint8_t room);

    /**
     * initiates loading the next floor with a nice success explosion animation
     */
    static void loadSuccess();

    /**
     * initiates reloading the same floor with a nice failure explosion animation
     * if this was the last live of the player, it will reload the room 0 of the current floor 
     * or the first floor depending on Options::checkpoints.
     */
    static void loadFailure();

};
