/**
 * @file OttoLee.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 1.0
 * @date 2021-01-26
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

#ifndef OTTOLEE_h
#define OTTOLEE_h

#include "OttoSound.h"
#include "OttoSensor.h"
#include "OttoServo.h"

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30

/**
 * @brief Otto Lee Driver
 * 
 */
class OttoLee: public OttoSound, public OttoSensor, public OttoServo
{
    private:
        
    public:
        OttoLee(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head, uint8_t pinNoiseSensor, uint8_t pinBuzzer, uint8_t pinUSTrigger, uint8_t pinUSEcho);
        //-- Otto initialization
        void init(bool load_calibration);
        
        //-- Predetermined Motion Functions
        void jump(float steps=1, uint16_t T = 2000);
        void walk(float steps=4, uint16_t T=1000, int8_t dir = FORWARD, uint16_t armOsc=0, uint16_t headOsc=0);
        void turn(float steps=4, uint16_t T=2000, int8_t dir = LEFT, uint16_t armOsc=0, uint16_t headOsc=0);
        void bend(uint16_t steps, uint16_t T, int8_t dir=LEFT);
        void shakeLeg(uint16_t steps=1, uint16_t T = 3000, int8_t dir=RIGHT);
        void updown(float steps=1, uint16_t T=1000, uint16_t h = 20);
        void handsup();
        void handwave(int8_t dir =RIGHT);
        void swing(float steps=1, uint16_t T=1000, uint16_t h=20);
        void tiptoeSwing(float steps=1, uint16_t T=900, uint16_t h=20);
        void jitter(float steps=1, uint16_t T=500, uint16_t h=20);
        void ascendingTurn(float steps=1, uint16_t T=900, uint16_t h=20);
        void moonwalker(float steps=1, uint16_t T=900, uint16_t h=20, int dir=LEFT);
        void crusaito(float steps=1, uint16_t T=900, uint16_t h=20, int dir=FORWARD);
        void flapping(float steps=1, uint16_t T=1000, uint16_t h=20, int dir=FORWARD);
        void headNo(float steps=1, uint16_t T=1000);
};


#endif //OTTOLEE_h

