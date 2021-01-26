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

#include <stdint.h>
#include "Oscillator.h"

#define _NBR_OF_SERVO   7

/**
 * @brief Otto Servo Driver
 * 
 */
class OttoServo
{
    private:
        Oscillator _servo[_NBR_OF_SERVO];
        uint8_t _servo_pins[_NBR_OF_SERVO];
        //int _servo_trim[7];
        uint8_t _servo_position[_NBR_OF_SERVO];
        bool _isOttoResting;

        uint32_t _final_time;
        uint32_t _partial_time;
        float _increment[_NBR_OF_SERVO];

    protected:
        //-- Predetermined Motion Functions
        void execute(uint16_t A[_NBR_OF_SERVO], uint16_t O[_NBR_OF_SERVO], uint16_t T, double phase_diff[_NBR_OF_SERVO], float steps);
        //-- Attach & detach functions
        void attachServos();
        void detachServos();
    public:
        OttoServo(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head);
        ~OttoServo();
        void init(bool load_calibration);
        //-- Oscillator Trims
        void setTrims(int8_t legLeftTrim, int8_t legRightTrim, int8_t footLeftTrim, int8_t footRightTrim, int8_t armLeftTrim, int8_t armRightTrim, int8_t headTrim);
        void saveTrimsOnEEPROM();
        int8_t loadTrimsFromEEPROM(uint8_t servoNumber);
        //-- Predetermined Motion Functions
        void moveSingle(uint8_t position, uint8_t servo_number);
        void moveServos(uint32_t time, uint8_t  servo_target[]);
        void oscillateServos(uint16_t A[_NBR_OF_SERVO], uint16_t O[_NBR_OF_SERVO], uint16_t T, double phase_diff[_NBR_OF_SERVO], float cycle=1);
        //-- HOME = Otto at rest position
        void home(uint32_t time = 500);
};

#endif //OTTOSERVO_h
