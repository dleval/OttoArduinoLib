/**
 * @file OttoServo.h
 * @author David LEVAL (dleval@dle-dev.com)
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

#ifndef OTTOSERVO_h
#define OTTOSERVO_h

#include <Arduino.h>
#include <EEPROM.h>
#include <stdint.h>
#include "Oscillator.h"

/**
 * @brief Otto Servo Driver
 * 
 * @tparam N Number of Servo
 */
template <uint8_t N>
class OttoServo
{
    private:
        uint8_t _nbrOfServo;
        Oscillator _servo[N];
        uint8_t _servo_pins[N];
        //int _servo_trim[7];
        uint8_t _servo_position[N];
        bool _isOttoResting;

        uint32_t _final_time;
        uint32_t _partial_time;
        float _increment[N];

    protected:
        //-- Predetermined Motion Functions
        void execute(int16_t A[N], int16_t O[N], uint16_t T, double phase_diff[N], float steps);
        //-- Attach & detach functions
        void attachServos();
        void detachServos();
    public:
        OttoServo();
        void init(uint8_t *servoPin);
        void initServo(bool loadCalibration);
        //-- Oscillator Trims
        void setTrims(int8_t *trim);
        void saveTrimsOnEEPROM();
        int8_t loadTrimsFromEEPROM(uint8_t servoNumber);
        //-- Predetermined Motion Functions
        void moveSingle(uint8_t position, uint8_t servo_number);
        void moveServos(uint32_t time, uint8_t  servo_target[]);
        void oscillateServos(int16_t A[N], int16_t O[N], uint16_t T, double phase_diff[N], float cycle=1);
        //-- HOME = Otto at rest position
        void home(uint32_t time = 500);
};


/**
 * @brief Construct a new Otto Servo< N>:: Otto Servo object
 * 
 * @tparam N Number of Servo
 */
template <uint8_t N>
OttoServo<N>::OttoServo()
{
}

/**
 * @brief Initialization of the Servo pins
 * 
 * @tparam N Number of Servo
 * @param servoPin  Servo pin Array
 */
template <uint8_t N>
void OttoServo<N>::init(uint8_t *servoPin)
{
    for (uint8_t i=0; i<N; i++) {
        _servo_pins[i] = servoPin[i];
        _servo_position[i] = 90; 
    }
}

/**
 * @brief Initialization of the Servo control system
 * 
 * @tparam N Number of Servo
 * @param loadCalibration Activates the recovery of calibrations in EEPROM
 */
template <uint8_t N>
void OttoServo<N>::initServo(bool loadCalibration)
{
    attachServos();
    _isOttoResting=false;

    if (loadCalibration) {
        for (uint8_t i=0; i<N; i++) {
            int8_t servo_trim = loadTrimsFromEEPROM(i);
            _servo[i].SetTrim(servo_trim);
        }
    }

    home();
}

/** Attach & Detach Servo(s) **************************************************/

/**
 * @brief Attach all Servo
 * 
 * @tparam N Number of Servo
 */
template <uint8_t N>
void OttoServo<N>::attachServos()
{
    for (uint8_t i = 0; i < N; i++) {
        _servo[i].attach(_servo_pins[i]);
    }
}

/**
 * @brief Detach all Servo
 * 
 * @tparam N Number of Servo
 */
template <uint8_t N>
void OttoServo<N>::detachServos()
{
    for (uint8_t i = 0; i < N; i++) {
        _servo[i].detach();
    }
}

/** Trims Servo(s) & EEPROM ***************************************************/

/**
 * @brief Set trim for all Servo
 * 
 * @tparam N Number of Servo
 * @param trim trim values Array
 */
template <uint8_t N>
void OttoServo<N>::setTrims(int8_t *trim)
{
    for (uint8_t i = 0; i < N; i++) {
        _servo[i].SetTrim(trim[i]);
    }
}

/**
 * @brief Save all trims on EEPROM
 * 
 * @tparam N Number of Servo
 */
template <uint8_t N>
void OttoServo<N>::saveTrimsOnEEPROM() 
{
    for (uint8_t i = 0; i < N; i++){ 
        EEPROM.write(i, (_servo[i].getTrim() + 90));
    } 
}

/**
 * @brief Read trim for one Servo
 * 
 * @tparam N Number of Servo
 * @param servoNumber 
 * @return int8_t trim value
 */
template <uint8_t N>
int8_t OttoServo<N>::loadTrimsFromEEPROM(uint8_t servoNumber)
{
    int8_t servoTrim = (EEPROM.read(servoNumber) - 90);
    if(servoTrim > 127) servoTrim = 127;
    if(servoTrim < -127) servoTrim = -127;

    return servoTrim;
}

/** Basic motion functions ****************************************************/

/**
 * @brief 
 * 
 * @tparam N Number of Servo
 * @param time 
 * @param servo_target 
 */
template <uint8_t N>
void OttoServo<N>::moveServos(uint32_t time, uint8_t  servo_target[]) 
{
    attachServos();
    if(_isOttoResting == true){
        _isOttoResting = false;
    }

    if(time>10){
        for (int i = 0; i < N; i++) _increment[i] = ((servo_target[i]) - _servo_position[i]) / (time / 10.0);
        _final_time =  millis() + time;

        for (int iteration = 1; millis() < _final_time; iteration++) {
        _partial_time = millis() + 10;
        for (int i = 0; i < N; i++) _servo[i].SetPosition(_servo_position[i] + (iteration * _increment[i]));
        while (millis() < _partial_time); //pause
        }
    }
    else{
        for (int i = 0; i < N; i++) _servo[i].SetPosition(servo_target[i]);
    }
    for (int i = 0; i < N; i++) _servo_position[i] = servo_target[i];
}

/**
 * @brief 
 * 
 * @tparam N 
 * @param position 
 * @param servo_number 
 */
template <uint8_t N>
void OttoServo<N>::moveSingle(uint8_t position, uint8_t servo_number) 
{
    if (position > 180) position = 90;
    if (position < 0) position = 90;
    attachServos();
    if(_isOttoResting == true) _isOttoResting = false;
    if(servo_number >= N) return;
    _servo[servo_number].SetPosition(position);
}

/**
 * @brief 
 * 
 * @tparam N 
 * @param A 
 * @param O 
 * @param T 
 * @param phase_diff 
 * @param cycle 
 */
template <uint8_t N>
void OttoServo<N>::oscillateServos(int16_t A[N], int16_t O[N], uint16_t T, double phase_diff[N], float cycle)
{

  for (int i=0; i<N; i++) {
    _servo[i].SetO(O[i]);
    _servo[i].SetA(A[i]);
    _servo[i].SetT(T);
    _servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()) {
     for (int i=0; i<N; i++){
        _servo[i].refresh();
     }
  }
}

/**
 * @brief 
 * 
 * @tparam N 
 * @param A 
 * @param O 
 * @param T 
 * @param phase_diff 
 * @param steps 
 */
template <uint8_t N>
void OttoServo<N>::execute(int16_t A[N], int16_t O[N], uint16_t T, double phase_diff[N], float steps)
{
    attachServos();
    if(_isOttoResting == true) _isOttoResting = false;

    int cycles=(int)steps;    

    //-- Execute complete cycles
    if (cycles >= 1) {
        for(int i = 0; i < cycles; i++) {
            oscillateServos(A,O, T, phase_diff);
        }
    }
        
    //-- Execute the final not complete cycle    
    oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}

/** Home position *************************************************************/

/**
 * @brief 
 * 
 * @tparam N 
 * @param time 
 */
template <uint8_t N>
void OttoServo<N>::home(uint32_t time)
{
    uint8_t homes[N];

    if(_isOttoResting == false) { //Go to rest position only if necessary
        for(uint8_t i=0; i<N; i++) homes[i] = 90; //All the servos at rest position
        moveServos(time,homes); 

        detachServos();
        _isOttoResting = true;
    }
}

#endif //OTTOSERVO_h
