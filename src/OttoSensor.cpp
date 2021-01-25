/**
 * @file OttoSensor.cpp
 * @author David LEVAL (dleval@senstronic.com)
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
#include "OttoSensor.h"

OttoSensor::OttoSensor(uint8_t pinNoiseSensor)
{
    _pinNoiseSensor = pinNoiseSensor;
    pinMode(_pinNoiseSensor,INPUT);
}

OttoSensor::~OttoSensor()
{
}

/**
 * @brief 
 * 
 */
void OttoSensor::init(uint8_t USTrigger, uint8_t USEcho)
{
    //US sensor init with the pins:
    _us.init(USTrigger, USEcho);

    randomSeed(analogRead(_pinNoiseSensor));
}

/**
 * @brief return Otto's noise sensor measure
 * 
 * @return int noise measurement
 */
uint16_t OttoSensor::getNoise()
{
    uint16_t noiseLevel = 0;
    uint16_t noiseReadings = 0;

    noiseLevel = analogRead(_pinNoiseSensor);

    for(int i=0; i<2; i++) {
        noiseReadings += analogRead(_pinNoiseSensor);
        delay(4); // delay in between reads for stability
    }

    noiseLevel = noiseReadings / 2;

    return noiseLevel;
}

/**
 * @brief return Otto's ultrasonic sensor measure
 * 
 * @return float distance measurement
 */
float OttoSensor::getDistance()
{
    return _us.read();
}
