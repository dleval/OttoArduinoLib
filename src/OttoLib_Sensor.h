/**
 * @file    OttoLib_Sensor.h
 * @author  David LEVAL (dleval@senstronic.com)
 * @brief   Otto distance sensor driver (Header)
 * @version 1.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef OTTOSENSOR_h
#define OTTOSENSOR_h

#include <stdint.h>
#include <US.h>

class OttoSensor
{
private:
    uint8_t _pinNoiseSensor;
    US _us;
public:
    OttoSensor(uint8_t pinNoiseSensor);
    ~OttoSensor();
    void init(uint8_t USTrigger, uint8_t USEcho);
    uint16_t getNoise();      //Noise Sensor
    float getDistance(); //US sensor
};




#endif //OTTOSENSOR_h
