/**
 * @file OttoServo.cpp
 * @author David LEVAL (dleval@dle-dev.com)
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

#include <Arduino.h>
#include <EEPROM.h>
#include "OttoServo.h"

/**
 * @brief Construct a new Otto Servo:: Otto Servo object
 * 
 * @param legLeft   Left leg pin Servo
 * @param legRight  Right leg pin Servo
 * @param footLeft  Left foot pin Servo
 * @param footRight Right foot pin Servo
 * @param armLeft   Left arm pin Servo
 * @param armRight  Right arm pin Servo
 * @param head      Head pin Servo
 */
OttoServo::OttoServo(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head)
{
    _servo_pins[0] = legLeft;
    _servo_pins[1] = legRight;
    _servo_pins[2] = footLeft;
    _servo_pins[3] = footRight;
    _servo_pins[4] = armLeft;
    _servo_pins[5] = armRight;
    _servo_pins[6] = head;
}

/**
 * @brief Destroy the Otto Servo:: Otto Servo object
 * 
 */
OttoServo::~OttoServo()
{
}

/**
 * @brief Initialization of the Servo control system
 * 
 * @param load_calibration Activates the recovery of calibrations in EEPROM
 */
void OttoServo::init(bool load_calibration)
{
    attachServos();
    _isOttoResting=false;

    if (load_calibration) {
        for (uint8_t i=0; i<_NBR_OF_SERVO; i++) {
            int8_t servo_trim = loadTrimsFromEEPROM(i);
            _servo[i].SetTrim(servo_trim);
        }
    }

    for (int i = 0; i < _NBR_OF_SERVO; i++) _servo_position[i] = 90; 
}

/** Attach & Detach Servo(s) **************************************************/

/**
 * @brief 
 * 
 */
void OttoServo::attachServos()
{
    // Serial.print("Attach Servo : ");
    for (uint8_t i = 0; i < _NBR_OF_SERVO; i++) {
        _servo[i].attach(_servo_pins[i]);
        // Serial.print(_servo_pins[i]);
    }
    // Serial.println("");
}

/**
 * @brief 
 * 
 */
void OttoServo::detachServos()
{
    // Serial.print("Detach Servo : ");
    for (uint8_t i = 0; i < _NBR_OF_SERVO; i++) {
        _servo[i].detach();
        // Serial.print(_servo_pins[i]);
    }
    // Serial.println("");
}

/** Trims Servo(s) & EEPROM ***************************************************/

/**
 * @brief 
 * 
 * @param legLeftTrim 
 * @param legRightTrim 
 * @param footLeftTrim 
 * @param footRightTrim 
 * @param armLeftTrim 
 * @param armRightTrim 
 * @param headTrim 
 */
void OttoServo::setTrims(int8_t legLeftTrim, int8_t legRightTrim, int8_t footLeftTrim, int8_t footRightTrim, int8_t armLeftTrim, int8_t armRightTrim, int8_t headTrim)
{
    _servo[0].SetTrim(legLeftTrim);
    _servo[1].SetTrim(legRightTrim);
    _servo[2].SetTrim(footLeftTrim);
    _servo[3].SetTrim(footRightTrim);
    _servo[4].SetTrim(armLeftTrim);
    _servo[5].SetTrim(armRightTrim);
    _servo[6].SetTrim(headTrim);
}

/**
 * @brief 
 * 
 */
void OttoServo::saveTrimsOnEEPROM() 
{
    for (int i = 0; i < _NBR_OF_SERVO; i++){ 
        EEPROM.write(i, _servo[i].getTrim());
    } 
}

/**
 * @brief 
 * 
 * @param servoNumber 
 * @return int 
 */
int8_t OttoServo::loadTrimsFromEEPROM(uint8_t servoNumber)
{
    int servoTrim = EEPROM.read(servoNumber);
    if(servoTrim > 127) servoTrim = 127;
    if(servoTrim < -127) servoTrim = -127;

    return servoTrim;
}

/** Basic motion functions ****************************************************/

/**
 * @brief 
 * 
 * @param time 
 * @param servo_target 
 */
void OttoServo::moveServos(uint32_t time, uint8_t  servo_target[]) 
{
    attachServos();
    if(_isOttoResting == true){
        _isOttoResting = false;
    }

    if(time>10){
        for (int i = 0; i < _NBR_OF_SERVO; i++) _increment[i] = ((servo_target[i]) - _servo_position[i]) / (time / 10.0);
        _final_time =  millis() + time;

        for (int iteration = 1; millis() < _final_time; iteration++) {
        _partial_time = millis() + 10;
        for (int i = 0; i < _NBR_OF_SERVO; i++) _servo[i].SetPosition(_servo_position[i] + (iteration * _increment[i]));
        while (millis() < _partial_time); //pause
        }
    }
    else{
        for (int i = 0; i < _NBR_OF_SERVO; i++) _servo[i].SetPosition(servo_target[i]);
    }
    for (int i = 0; i < _NBR_OF_SERVO; i++) _servo_position[i] = servo_target[i];
}

/**
 * @brief 
 * 
 * @param position 
 * @param servo_number 
 */
void OttoServo::moveSingle(uint8_t position, uint8_t servo_number) 
{
    if (position > 180) position = 90;
    if (position < 0) position = 90;
    attachServos();
    if(_isOttoResting == true) _isOttoResting = false;
    if(servo_number >= _NBR_OF_SERVO) return;
    _servo[servo_number].SetPosition(position);
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
void OttoServo::oscillateServos(int16_t A[_NBR_OF_SERVO], int16_t O[_NBR_OF_SERVO], uint16_t T, double phase_diff[_NBR_OF_SERVO], float cycle)
{

  for (int i=0; i<_NBR_OF_SERVO; i++) {
    _servo[i].SetO(O[i]);
    _servo[i].SetA(A[i]);
    _servo[i].SetT(T);
    _servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()) {
     for (int i=0; i<_NBR_OF_SERVO; i++){
        _servo[i].refresh();
     }
  }
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
void OttoServo::execute(int16_t A[_NBR_OF_SERVO], int16_t O[_NBR_OF_SERVO], uint16_t T, double phase_diff[_NBR_OF_SERVO], float steps = 1.0)
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

void OttoServo::home(uint32_t time)
{
    uint8_t homes[_NBR_OF_SERVO];

    if(_isOttoResting == false) { //Go to rest position only if necessary
        for(uint8_t i=0; i<_NBR_OF_SERVO; i++) homes[i] = 90; //All the servos at rest position
        moveServos(time,homes); 

        detachServos();
        _isOttoResting = true;
    }
}

