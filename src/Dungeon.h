#pragma once

#include <stdint.h>

/**
 * Main class of the game, but it only handles
 *  - initiation of all static classes
 *  - initiation of io
 *  - timing for the updates
 *  - starting the idle and options level
 */
class Dungeon {

public:

    // # ================================================================ #
    // # ::  UPDATE                                                    :: #
    // # ================================================================ #

    /**
     * initiates the dungeon, static classes and io
     */
    static void setup();

private:
    /**
     * busy waits at the end of every loop to ensure that the game 
     * averages an update time of UPDATE_TIME ms.
     * 
     * @param loop_start_time 
     *      time stamp of when the current loop (update iteration) started
     */
    static void ensureFrameTiming(uint16_t &loop_start_time);

public:
    /**
     * contains the game loop.
     * This method fill run forever.
     * Exception: in Simulator - mode, it can be terminated by pressing 'q'
     */
    static void run();
    
    // # ================================================================ #
    // # ::  STATE                                                     :: #
    // # ================================================================ #

    /**
     * @return 
     *      whether the dungeon is currently idle
     */
    static bool isIdle();

    /**
     * @return 
     *      whether the dungeon is currently idle
     */
    static bool isSecret();

    /**
     * @return 
     *      whether the dungeon is currently in options mode
     */
    static bool isOptions();

    /**
     * @return 
     *      whether the dungeon is currently in victory mode
     */
    static bool isVictory();

    /**
     * @return 
     *      whether the hero reached the goal
     */
    static bool isSuccess();

    /**
     * @return 
     *      whether the hero died
     */
    static bool isFailure();

};
