/**
 * @file    OttoLib_Sensor.cpp
 * @author  David LEVAL (dleval@senstronic.com)
 * @brief   Otto distance sensor driver
 * @version 1.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include "OttoLib_Sensor.h"

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
