/**
 * @file OttoServo.h
 * @author David LEVAL (dleval@dle-dev.com)
 * @brief 
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

#ifndef OTTOSERVO_h
#define OTTOSERVO_h

#include <stdint.h>
#include "Oscillator.h"

/** Typedef *******************************************************************/

#define _NBR_OF_SERVO 7

typedef enum {
    LEG_L = 0, 
    LEG_R, 
    FOOT_L, 
    FOOT_R, 
    ARM_L, 
    ARM_R, 
    HEAD  
}ottoLeeServo_TEnum;

/** Macro *********************************************************************/

/**
 * @brief List of Servo control pins : 
 *        legLeft, legRight, footLeft, footRight, armLeft, armRight, head
 * 
 */
#define OTTO_LEE_PIN_TYPE   const ottoLeePinServo_TStruct

/** Class *********************************************************************/

/**
 * @brief Otto Servo
 * 
 */
class OttoServo
{
private:
    // Oscillator _servo[_NBR_OF_SERVO];
    uint8_t _pinServo[_NBR_OF_SERVO];
    uint8_t _servo_position[_NBR_OF_SERVO];
    //-- Attach & detach functions
    void _attachServos();
    void _detachServos();
    
protected:
    bool isOttoResting;
    //-- Predetermined Motion Functions
    void execute(int A[_NBR_OF_SERVO], int O[_NBR_OF_SERVO], int T, double phase_diff[_NBR_OF_SERVO], float steps = 1.0);

public:
    // OttoServo(ottoLeePinServo_TStruct *ottoLeePinServo);
    OttoServo(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head);
    ~OttoServo();
    void init(bool loadCalibration);
    //-- Oscillator Trims
    void setTrims(int legLeftTrim, int legRightTrim, int footLeftTrim, int footRightTrim, int armLeftTrim, int armRightTrim, int headTrim);
    int loadTrimsFromEEPROM(uint8_t servoNumber);
    void saveTrimsOnEEPROM();
    //-- Predetermined Motion Functions
    void moveSingle(int position, uint8_t servo_number);
    void moveServos(uint32_t time, int  servo_target[]);
    void oscillateServos(int A[_NBR_OF_SERVO], int O[_NBR_OF_SERVO], int T, double phase_diff[_NBR_OF_SERVO], float cycle=1);
    //-- HOME = Otto at rest position
    void home(uint32_t duration = 500);
};

#endif //OTTOSERVO_h
