/**
 * @file OttoServo.cpp
 * @author David LEVAL (dleval@dle-dev.com)
 * @version 1.0
 * @date 2021-01-22
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

#include <Arduino.h>
#include <EEPROM.h>
#include "OttoServo.h"

Oscillator _servo[_NBR_OF_SERVO];

/**
 * @brief Construct a new Otto Servo:: Otto Servo object
 * 
 * @param legLeft 
 * @param legRight 
 * @param footLeft 
 * @param footRight 
 * @param armLeft 
 * @param armRight 
 * @param head 
 */
OttoServo::OttoServo(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head)
{
    _pinServo[LEG_L] = legLeft;
    _pinServo[LEG_R] = legRight;
    _pinServo[FOOT_L] = footLeft;
    _pinServo[FOOT_R] = footRight;
    _pinServo[ARM_L] = armLeft;
    _pinServo[ARM_R] = armRight;
    _pinServo[HEAD] = head;
}

/**
 * @brief Destroy the Otto Servo:: Otto Servo object
 * 
 */
OttoServo::~OttoServo()
{
}


/**
 * @brief Attach all oscillator to all servo
 * 
 */
void OttoServo::_attachServos()
{
    for(uint8_t i; i<_NBR_OF_SERVO; i++) _servo[i].attach((int)_pinServo[i]);
}

/**
 * @brief Detach all oscillator to all servo
 * 
 */
void OttoServo::_detachServos()
{
    for(uint8_t i; i<_NBR_OF_SERVO; i++) _servo[i].detach();
}

/**
 * @brief Initialization of the Servo control system
 * 
 * @param loadCalibration Activates the recovery of calibrations in EEPROM
 */
void OttoServo::init(bool loadCalibration)
{
    _attachServos();
    isOttoResting=false;

    if (loadCalibration) {
        for (uint8_t i = 0; i < 7; i++) {
            int servo_trim = loadTrimsFromEEPROM(i);
            _servo[i].SetTrim(servo_trim);
        }
    }

    for (int i = 0; i < 7; i++) _servo_position[i] = 90;
}

/**
 * @brief Adjusting a Servo Position 
 * 
 * @param position      Angle degree (0 to 180)
 * @param servoNumber   Servo Identification
 */
void OttoServo::moveSingle(int position, uint8_t servoNumber) 
{
    if (position > 180) position = 90;
    if (position < 0) position = 90;
    _attachServos();
    if(isOttoResting) isOttoResting = false;

    _servo[servoNumber].SetPosition(position);
}

/**
 * @brief Servo calibration offset adjustment
 * 
 * @param legLeft 
 * @param legRight 
 * @param footLeft 
 * @param footRight 
 * @param armLeft 
 * @param armRight 
 * @param head 
 */
void OttoServo::setTrims(int legLeftTrim, int legRightTrim, int footLeftTrim, int footRightTrim, int armLeftTrim, int armRightTrim, int headTrim)
{ 
    _servo[LEG_L].SetTrim(legLeftTrim);
    _servo[LEG_R].SetTrim(legRightTrim);
    _servo[FOOT_L].SetTrim(footLeftTrim);
    _servo[FOOT_R].SetTrim(footRightTrim);
    _servo[ARM_L].SetTrim(armLeftTrim);
    _servo[ARM_R].SetTrim(armRightTrim);
    _servo[HEAD].SetTrim(headTrim);
}

/**
 * @brief Save calibration offsets in EEPROM
 * 
 */
void OttoServo::saveTrimsOnEEPROM() 
{
    for (int i = 0; i < 4; i++){ 
        EEPROM.write(i, _servo[i].getTrim());
    } 
}

/**
 * @brief 
 * 
 * @param servoNumber 
 * @return int 
 */
int OttoServo::loadTrimsFromEEPROM(uint8_t servoNumber)
{
    int servoTrim = EEPROM.read(servoNumber);
    if(servoTrim > 127) servoTrim = 127;
    if(servoTrim < -127) servoTrim = -127;

    return servoTrim;
}

/**
 * @brief 
 * 
 * @param A 
 * @param O 
 * @param T 
 * @param phase_diff 
 * @param steps 
 */
void OttoServo::execute(int A[_NBR_OF_SERVO], int O[_NBR_OF_SERVO], int T, double phase_diff[_NBR_OF_SERVO], float steps)
{
    _attachServos();
    if(isOttoResting) isOttoResting = false;

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

/**
 * @brief 
 * 
 * @param time 
 * @param servo_target 
 */
void OttoServo::moveServos(uint32_t time, int  servo_target[]) 
{
    float _increment[7];
    uint32_t _final_time;
    uint32_t _partial_time;

    _attachServos();
    if(isOttoResting) isOttoResting = false;

    if(time>10){
        for (int i = 0; i < 7; i++) _increment[i] = ((servo_target[i]) - _servo_position[i]) / (time / 10.0);
        _final_time =  millis() + time;

        for (int iteration = 1; millis() < _final_time; iteration++) {
        _partial_time = millis() + 10;
        for (int i = 0; i < 7; i++) _servo[i].SetPosition(_servo_position[i] + (iteration * _increment[i]));
        while (millis() < _partial_time); //pause
        }
    }
    else{
        for (int i = 0; i < 7; i++) _servo[i].SetPosition(servo_target[i]);
    }
    for (int i = 0; i < 7; i++) _servo_position[i] = servo_target[i];
}

/**
 * @brief 
 * 
 * @param A 
 * @param O 
 * @param T 
 * @param phase_diff 
 * @param cycle 
 */
void OttoServo::oscillateServos(int A[_NBR_OF_SERVO], int O[_NBR_OF_SERVO], int T, double phase_diff[_NBR_OF_SERVO], float cycle)
{

  for (uint8_t i=0; i<_NBR_OF_SERVO; i++) {
    _servo[i].SetO(O[i]);
    _servo[i].SetA(A[i]);
    _servo[i].SetT(T);
    _servo[i].SetPh(phase_diff[i]);
  }
  double ref = (double)millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()){
     for (uint8_t i=0; i<_NBR_OF_SERVO; i++){
        _servo[i].refresh();
     }
  }
}

/**
 * @brief Set all servos in home position
 * 
 * @param duration Duration of return to home position
 */
void OttoServo::home(uint32_t duration)
{
    if(duration == 0) duration = 500;

    if(isOttoResting == false) { //Check if all servo are already in home position
        int homes[7] = {90, 90, 90, 90, 90, 90, 90}; //All the servos at rest position
        moveServos(duration,homes);   //Move the servos
        _detachServos();
        isOttoResting = true;
    }
}

