/**
 * @file    OttoLib_Mouths.h
 * @author  David LEVAL (dleval@dle-dev.com)
 * @brief   
 * @version 1.1
 * 
 * @copyright Copyright (c) 2022
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

#ifndef __OTTO_LIB_MOUTHS_h
#define __OTTO_LIB_MOUTHS_h

#include <stdint.h>

typedef enum {
    zero = 0,
    one,
    two,
    three, 
    four,
    five,
    six,
    seven,
    eight, 
    nine,
    smile,                  
    happyOpen,  
    happyClosed, 
    heart,
    bigSurprise, 
    smallSurprise,
    tongueOut,
    vamp1, 
    vamp2, 
    lineMouth,
    confused, 
    diagonal,         
    sad,
    sadOpen,
    sadClosed,
    okMouth,
    xMouth,
    interrogation,
    thunder,
    culito,
    angry,
}ottoMouths_t;

typedef enum {
    littleUuh = 0,
    dreamMouth,  
    adivinawi,
    wave,
}ottoAnimationMouths_t;

class OttoMouth
{
    private:
        uint8_t _pinCS;
    public:
        OttoMouth(uint8_t pinCS);
        ~OttoMouth();
        void putMouth(ottoMouths_t mouth, bool predefined = true);
        void putAnimationMouth(ottoAnimationMouths_t anim, int index);
        void clearMouth();
        void initMATRIX(int DIN, int CS, int CLK, int rotate);
        void matrixIntensity(int intensity);
        void setLed(byte X, byte Y, byte value);
        void writeText (const char * s, byte scrollspeed);
};

#endif //__OTTO_LIB_MOUTHS_h
