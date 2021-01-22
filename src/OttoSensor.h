/**
 * @file OttoSensor.h
 * @author David LEVAL (dleval@senstronic.com)
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
