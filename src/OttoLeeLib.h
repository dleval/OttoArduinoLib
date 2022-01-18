/**
 * @file    OttoLeeLib.h
 * @author  David LEVAL (dleval@dle-dev.com)
 * @brief   Otto Lee robot driver (Header)
 * @version 1.1
 * 
 * @copyright Copyright (c) 2022
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

#include "OttoGlobal.h"
#include "OttoLib_Sound.h"
#include "OttoLib_Sensor.h"
#include "OttoLib_Servo.h"

#define _NBR_OF_SERVO   7



/**
 * @brief Otto Lee Driver
 * 
 */
class OttoLee: public OttoSound, public OttoSensor, public OttoServo<_NBR_OF_SERVO>
{
    private:
        
    public:
        OttoLee(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head, uint8_t pinNoiseSensor, uint8_t pinBuzzer, uint8_t pinUSTrigger, uint8_t pinUSEcho);
        //-- Otto initialization
        void init(bool load_calibration);
        
        //-- Predetermined Motion Functions
        void jump(float steps=1, uint16_t T = 2000);
        void walk(float steps=4, uint16_t T=1000, int8_t dir = FORWARD, int16_t armOsc=0, int16_t headOsc=0);
        void turn(float steps=4, uint16_t T=2000, int8_t dir = LEFT, int16_t armOsc=0, int16_t headOsc=0);
        void bend(uint16_t steps, uint16_t T, int8_t dir=LEFT);
        void shakeLeg(uint16_t steps=1, uint16_t T = 3000, int8_t dir=RIGHT);
        void updown(float steps=1, uint16_t T=1000, int16_t h = 20);
        void handsup();
        void handwave(int8_t dir =RIGHT);
        void swing(float steps=1, uint16_t T=1000, int16_t h=20);
        void tiptoeSwing(float steps=1, uint16_t T=900, int16_t h=20);
        void jitter(float steps=1, uint16_t T=500, int16_t h=20);
        void ascendingTurn(float steps=1, uint16_t T=900, int16_t h=20);
        void moonwalker(float steps=1, uint16_t T=900, int16_t h=20, int dir=LEFT);
        void crusaito(float steps=1, uint16_t T=900, int16_t h=20, int dir=FORWARD);
        void flapping(float steps=1, uint16_t T=1000, int16_t h=20, int dir=FORWARD);
        void headNo(float steps=1, uint16_t T=1000);
};


#endif //OTTOLEE_h

