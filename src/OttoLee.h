/**
 * @file OttoLee.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef OttoLee_h
#define OttoLee_h

#include <Servo.h>
#include <Oscillator.h>
#include <US.h>
#include <EEPROM.h>
#include <TimerFreeTone.h>
#include <Otto_sound9.h>

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30


class OttoLee
{
    private:
        Oscillator _servo[7];
        US _us;
        uint8_t _servo_pins[7];
        //int _servo_trim[7];
        uint8_t _servo_position[7];
        uint8_t _pinBuzzer;
        uint8_t _pinNoiseSensor;
        bool _isOttoResting;

        unsigned long _final_time;
        unsigned long _partial_time;
        float _increment[7];

        //-- Predetermined Motion Functions
        
        void _moveSingle(int position, int servo_number);
        void _execute(int A[7], int O[7], int T, double phase_diff[7], float steps);
        //-- Sounds
        void _tone (float noteFrequency, long noteDuration, int silentDuration);
    public:
        //-- Otto initialization
        void init(uint8_t LEG_L, uint8_t LEG_R, uint8_t FOOT_L, uint8_t FOOT_R, uint8_t ARM_L, uint8_t ARM_R, uint8_t HEAD, bool load_calibration, uint8_t NoiseSensor, uint8_t Buzzer, uint8_t USTrigger, uint8_t USEcho);
        //-- Attach & detach functions
        void attachServos();
        void detachServos();
        //-- Oscillator Trims
        void setTrims(int8_t LEG_L, int8_t LEG_R, int8_t FOOT_L, int8_t FOOT_R, int8_t ARM_L, int8_t ARM_R, int8_t HEAD);
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
        //-- Sounds
        void bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
        void sing(int songName);
};


#endif //OttoLee_h

