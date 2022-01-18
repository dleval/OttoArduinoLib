/**
 * @file    OttoLib_Sound.h
 * @author  David LEVAL (dleval@dle-dev.com)
 * @brief   Otto sound driver (Header)
 * @version 1.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OTTOSOUND_h
#define OTTOSOUND_h

#include <stdint.h>
#include "OttoSoundNote.h"

/** Configuration *************************************************************/
// #define __USE_ARDUINO_TONE_LIB      1
#define POLY_DELTA  14

/* Song List ******************************************************************/
#define S_connection 	    0
#define S_disconnection     1
#define S_buttonPushed 	    2
#define S_mode1 		    3
#define S_mode2 		    4
#define S_mode3 		    5
#define S_surprise 		    6
#define S_OhOoh 		    7
#define S_OhOoh2 		    8
#define S_cuddly 		    9
#define S_sleeping 		    10
#define S_happy 		    11
#define S_superHappy 	    12
#define S_happy_short 	    13
#define S_sad 			    14
#define S_confused 		    15
#define S_fart1			    16
#define S_fart2			    17
#define S_fart3			    18

/******************************************************************************/

/**
 * @brief Otto Robot Sound generation
 * 
 */
class OttoSound
{
private:
    uint8_t _pinBuzzer;
    void _r2d2Phrase1();
    void _r2d2Phrase2();
protected:
    void _tone (float noteFrequency, uint32_t noteDuration, uint16_t silentDuration, uint8_t volume = 10);
public:
    OttoSound(uint8_t pinBuzzer);
    ~OttoSound();
    void bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(uint8_t songName);
    void r2d2();
    void songSilentNight(void);
    void songTetris(void);
};

#endif //OTTOSOUND_h
