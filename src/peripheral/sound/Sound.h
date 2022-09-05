#pragma once

#include <Platform.h>

/**
 * static class which handles playing sound.
 */
class Sound {

public:

    /**
     * The different sounds.
     * A sound with a lower int-value can cut off other sounds.
     */
    enum Type : uint8_t {
        VICTORY,
        SUCCESS,
        FAILURE,
        SECRET,
        TELEPORT,
        ITEM,
        ATTACK,
        SILENT
    };

private:
#ifdef ENABLE_SOUND
    static uint8_t type;
    static uint8_t idx;
    static int8_t arg;
    static uint8_t duration;
    static uint8_t bit;

    /**
     * the secret morse code
     * Encoding:
     * 0: short
     * 10: long
     * 11: pause
     */
    static const uint8_t secret_code[] PROGMEM;
    /**
     * attack sound
     */
    static const uint8_t attack[] PROGMEM;
    /**
     * failure sound
     */
    static const uint8_t failure[] PROGMEM;
    /**
     * item sound
     */
    static const uint8_t item[] PROGMEM;
    /**
     * success sound
     */
    static const uint8_t success[] PROGMEM;
    /**
     * teleport sound
     */
    static const uint8_t teleport[] PROGMEM;
    /**
     * victory sound
     */
    static const uint8_t victory[] PROGMEM;

    /**
     * plays a sound as defined in a PROGMEM array of notes
     * 
     * @param ptr 
     *      PROGMEM pointer to the note-array
     * @param count 
     *      how many notes there are in the sound
     * @param duration 
     *      how long to play the current note
     * @return uint16_t 
     *      the frequency to play
     */
    static uint16_t playProgmem(const uint8_t *ptr, uint8_t count, uint8_t duration);

    /**
     * plays the secret code
     * 
     * @return uint16_t
     *      the frequency to play
     */
    static uint16_t playSecret();
#endif

public:

    /**
     * initiates the sound to playing nothing
     */
    static void init();

    /**
     * updates the PWM configuration to generate the current sound
     */
    static void update();

    /**
     * @return 
     *      the sound currently playing
     */
    static Type getType();

    /**
     * Start playing a new sound.
     * When another sound is already playing, the new sound will
     *  - be dropped if its int-value is larger
     *  - be started immediately otherwise
     * 
     * @param type 
     *    the next sound to play
     */
    static void play(Type type);

};
