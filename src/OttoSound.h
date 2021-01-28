/**
 * @file OttoSound.h
 * @author David LEVAL (dleval@dle-dev.com)
 * @brief 
 * @version 1.0
 * @date 2021-01-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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

#define SongSilentNight     128

/******************************************************************************/

/**
 * @brief Otto Robot Sound generation
 * 
 */
class OttoSound
{
private:
    uint8_t _pinBuzzer;
    void _tone (float noteFrequency, uint32_t noteDuration, uint16_t silentDuration, uint8_t volume = 10);
    void _r2d2Phrase1();
    void _r2d2Phrase2();
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
