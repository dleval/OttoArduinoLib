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

#include <Servo.h>
#include <Oscillator.h>
#include <US.h>
#include <EEPROM.h>
#include "OttoSound.h"

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30

typedef enum {
    LEG_L = 0, 
    LEG_R = 1, 
    FOOT_L = 2, 
    FOOT_R = 3, 
    ARM_L = 4, 
    ARM_R = 5, 
    HEAD = 6
}ottoLeeServo_TEnum;

/**
 * @brief 
 * 
 */
class OttoLee : public OttoSound
{
    private:
        Oscillator _servo[7];
        US _us;
        uint8_t _pinServo[7];
        //int _servo_trim[7];
        uint8_t _servo_position[7];
        uint8_t _pinNoiseSensor;
        bool _isOttoResting;

        unsigned long _final_time;
        unsigned long _partial_time;
        float _increment[7];

        //-- Predetermined Motion Functions
        
        void _moveSingle(int position, int servo_number);
        void _execute(int A[7], int O[7], int T, double phase_diff[7], float steps = 1.0);
    public:
        OttoLee(const uint8_t *pinServo, uint8_t pinBuzzer);
        //-- Otto initialization
        void init(bool load_calibration, uint8_t NoiseSensor, uint8_t USTrigger, uint8_t USEcho);
        //-- Attach & detach functions
        void attachServos();
        void detachServos();
        //-- Oscillator Trims
        void setTrims(int8_t legLeft, int8_t legRight, int8_t footLeft, int8_t footRight, int8_t armLeft, int8_t armRight, int8_t head);
        void saveTrimsOnEEPROM();
        //-- Predetermined Motion Functions
        void _moveServos(int time, int  servo_target[]);
        void oscillateServos(int A[7], int O[7], int T, double phase_diff[7], float cycle=1);
        //-- HOME = Otto at rest position
        void home();
        bool getRestState();
        void setRestState(bool state);
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
        //-- Sensors functions
        float getDistance(); //US sensor
        int getNoise();      //Noise Sensor
};


#endif //OTTOLEE_h

