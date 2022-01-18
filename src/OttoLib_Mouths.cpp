#include <Arduino.h>
#include "OttoLib_Mouths.h"

//#include <OttoLib.h>

OttoMouth::OttoMouth(uint8_t pinCS)
{

}

OttoMouth::~OttoMouth()
{
}




void OttoMouth::putMouth(ottoMouths_t mouth, bool predefined = true)
{
    //TODO !!!
}

void OttoMouth::putAnimationMouth(ottoAnimationMouths_t anim, int index)
{
    //TODO !!!
}

void OttoMouth::clearMouth()
{
    //TODO !!!
}

void OttoMouth::initMATRIX(int DIN, int CS, int CLK, int rotate)
{
    //TODO !!!
}

void OttoMouth::matrixIntensity(int intensity)
{
    //TODO !!!
}
void OttoMouth::setLed(byte X, byte Y, byte value)
{
    //TODO !!!
}

void OttoMouth::writeText (const char * s, byte scrollspeed)
{
    //TODO !!!
}
