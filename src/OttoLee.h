/**
 * @file OttoLee.h
 * @author your name (you@domain.com)
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

#ifndef OTTOLEE_h
#define OTTOLEE_h

#include "OttoSound.h"
#include "OttoServo.h"
#include "OttoSensor.h"

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30


/**
 * @brief 
 * 
 */
class OttoLee : public OttoSound, public OttoServo, public OttoSensor
{
    private:
        

    public:
        OttoLee(const ottoLeePinServo_TStruct *ottoLeePinServo, uint8_t pinBuzzer, uint8_t pinNoiseSensor);
        //-- Otto initialization
        void init(bool loadCalibration, uint8_t USTrigger, uint8_t USEcho);
        //-- Predetermined Motion Functions
        void jump(float steps=1, int T = 2000);
        void walk(float steps=4, int T=1000, int dir = FORWARD, int armOsc=0, int headOsc=0);
        void turn(float steps=4, int T=2000, int dir = LEFT, int armOsc=0, int headOsc=0);
        void bend(int steps=1, int T=1400, int dir=LEFT);
        void shakeLeg(int steps=1, int T = 3000, int dir=RIGHT);
        void updown(float steps=1, int T=1000, int h = 20);
        void handsup();
        void handwave(int dir =RIGHT);
        void swing(float steps=1, int T=1000, int h=20);
        void tiptoeSwing(float steps=1, int T=900, int h=20);
        void jitter(float steps=1, int T=500, int h=20);
        void ascendingTurn(float steps=1, int T=900, int h=20);
        void moonwalker(float steps=1, int T=900, int h=20, int dir=LEFT);
        void crusaito(float steps=1, int T=900, int h=20, int dir=FORWARD);
        void flapping(float steps=1, int T=1000, int h=20, int dir=FORWARD);
        void headNo(float steps=1, int T=1000);
        
};


#endif //OTTOLEE_h

