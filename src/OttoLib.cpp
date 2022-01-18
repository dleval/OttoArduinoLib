/**
 * @file    Otto.cpp
 * @author  David LEVAL (dleval@dle-dev.com)
 * @brief   Otto robot driver
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

#include <Arduino.h>
#include <OttoLib.h>

Otto::Otto(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t pinNoiseSensor, uint8_t pinBuzzer, uint8_t pinUSTrigger, uint8_t pinUSEcho) : 
    OttoSound(pinBuzzer), OttoSensor(pinNoiseSensor), OttoServo()
{
    uint8_t servoPin[_NBR_OF_SERVO];

    OttoSensor::init(pinUSTrigger, pinUSEcho);

    servoPin[0] = legLeft;
    servoPin[1] = legRight;
    servoPin[2] = footLeft;
    servoPin[3] = footRight;

    OttoServo::init(servoPin);
}

Otto::Otto() :
    OttoSound(13), OttoSensor(A6), OttoServo()
{
    //TODO !!!
}

/**
 * @brief 
 * 
 * @param load_calibration 
 */
void Otto::init(bool load_calibration)
{
    OttoServo::initServo(load_calibration);
}

void Otto::init(int YL, int YR, int RL, int RR, bool load_calibration, int Buzzer)
{
    //TODO !!!
}


/** Predetermined motion sequences ********************************************/

/**
 * @brief Otto predetermined movement: Jump
 * 
 * @param steps Number of steps
 * @param T     Period
 */
void Otto::jump(float steps, uint16_t T)
{
    uint8_t up[_NBR_OF_SERVO]={90,90,150,30};
    _moveServos(T,up);
    uint8_t down[_NBR_OF_SERVO]={90,90,90,90};
    _moveServos(T,down);
}

/**
 * @brief Otto predetermined movement: Walking  (forward or backward) 
 * 
 * @param steps Number of steps
 * @param T Period
 * @param dir Direction: FORWARD / BACKWARD
 */
void Otto::walk(float steps, uint16_t T, int8_t dir)
{
    //-- Oscillator parameters for walking
    //-- Hip sevos are in phase
    //-- Feet servos are in phase
    //-- Hip and feet are 90 degrees out of phase
    //--      -90 : Walk forward
    //--       90 : Walk backward
    //-- Feet servos also have the same offset (for tiptoe a little bit)

    int16_t A[_NBR_OF_SERVO]= {30, 30, 20, 20};
    int16_t O[_NBR_OF_SERVO] = {0, 0, 4, -4};
    double phase_diff[_NBR_OF_SERVO] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps);  
}

/**
 * @brief Otto predetermined movement: Turning (left or right)
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param dir   Direction: LEFT / RIGHT
 */
void Otto::turn(float steps, uint16_t T, int8_t dir)
{
    //-- Same coordination than for walking (see Otto::walk)
    //-- The Amplitudes of the hip's oscillators are not igual
    //-- When the right hip servo amplitude is higher, the steps taken by
    //--   the right leg are bigger than the left. So, the robot describes an 
    //--   left arc
    int16_t A[_NBR_OF_SERVO]= {30, 30, 20, 20};
    int16_t O[_NBR_OF_SERVO] = {0, 0, 4, -4};
    double phase_diff[_NBR_OF_SERVO] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)}; 
        
    if (dir == LEFT) {  
        A[0] = 30; //-- Left hip servo
        A[1] = 10; //-- Right hip servo
    }
    else {
        A[0] = 10;
        A[1] = 30;
    }
        
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Lateral bend
 * 
 * @param steps Number of bends
 * @param T Period of one bend
 * @param dir RIGHT=Right bend LEFT=Left bend
 */
void Otto::bend(uint16_t steps, uint16_t T, int8_t dir)
{
    //Parameters of all the movements. Default: Left bend
    uint8_t bend1[_NBR_OF_SERVO]={90, 90, 62, 35}; 
    uint8_t bend2[_NBR_OF_SERVO]={90, 90, 62, 105};
    uint8_t homes[_NBR_OF_SERVO]={90, 90, 90, 90};

    //Time of one bend, constrained in order to avoid movements too fast.
    //T=max(T, 600);

    //Changes in the parameters if right direction is chosen 
    if(dir==-1) {
        bend1[2]=180-35;
        bend1[3]=180-60;  //Not 65. Otto is unbalanced
        bend2[2]=180-105;
        bend2[3]=180-60;
    }

    //Time of the bend movement. Fixed parameter to avoid falls
    uint16_t T2=800; 

    //Bend movement
    for (uint16_t i=0;i<steps;i++) {
        _moveServos(T2/2,bend1);
        _moveServos(T2/2,bend2);
        delay(T*0.8);
        _moveServos(500,homes);
    }
}


/**
 * @brief Otto predetermined movement: Shake a leg
 * 
 * @param steps Number of shakes
 * @param T     Period of one shake
 * @param dir   RIGHT=Right leg LEFT=Left leg
 */
void Otto::shakeLeg(uint16_t steps, uint16_t T, int8_t dir)
{
    //This variable change the amount of shakes
    uint16_t numberLegMoves=2;

    //Parameters of all the movements. Default: Right leg
    uint8_t shake_leg1[_NBR_OF_SERVO]={90, 90, 58, 35};   
    uint8_t shake_leg2[_NBR_OF_SERVO]={90, 90, 58, 120};
    uint8_t shake_leg3[_NBR_OF_SERVO]={90, 90, 58, 60};
    uint8_t homes[_NBR_OF_SERVO]={90, 90, 90, 90};

    //Changes in the parameters if left leg is chosen
    if(dir==-1) {
        shake_leg1[2]=180-35;
        shake_leg1[3]=180-58;
        shake_leg2[2]=180-120;
        shake_leg2[3]=180-58;
        shake_leg3[2]=180-60;
        shake_leg3[3]=180-58;
    }
  
    //Time of the bend movement. Fixed parameter to avoid falls
    uint16_t T2=2000;    
    //Time of one shake, constrained in order to avoid movements too fast.            
    T=T-T2;
    T=max(T,200*numberLegMoves);  

    for (uint16_t j=0; j<steps;j++) {
        //Bend movement
        _moveServos(T2/2,shake_leg1);
        _moveServos(T2/2,shake_leg2);
        //Shake movement
        for (uint16_t i=0;i<numberLegMoves;i++)
        {
        _moveServos(T/(2*numberLegMoves),shake_leg3);
        _moveServos(T/(2*numberLegMoves),shake_leg2);
        }
        _moveServos(500,homes); //Return to home position
    }
    
    delay(T);
}

/**
 * @brief Otto predetermined movement: up & down
 * 
 * @param steps Number of jumps
 * @param T     Period
 * @param h     Jump height: SMALL / MEDIUM / BIG (or a number in degrees 0 - 90)
 */
void Otto::updown(float steps, uint16_t T, int16_t h)
{
    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    int16_t A[_NBR_OF_SERVO]= {0, 0, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h, -h};
    double phase_diff[_NBR_OF_SERVO] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: swinging side to side
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     Amount of swing (from 0 to 50 aprox)
 */
void Otto::swing(float steps, uint16_t T, int16_t h)
{
    //-- Both feets are in phase. The offset is half the amplitude
    //-- It causes the robot to swing from side to side
    int16_t A[_NBR_OF_SERVO]= {0, 0, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h/2, -h/2};
    double phase_diff[_NBR_OF_SERVO] = {0, 0, DEG2RAD(0), DEG2RAD(0)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: swinging side to side without touching the floor with the heel
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     Amount of swing (from 0 to 50 aprox)
 */
void Otto::tiptoeSwing(float steps, uint16_t T, int16_t h){

    //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
    //-- It causes the robot to swing from side to side
    int16_t A[_NBR_OF_SERVO]= {0, 0, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h, -h};
    double phase_diff[_NBR_OF_SERVO] = {0, 0, 0, 0};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Jitter
 * 
 * @param steps Number of jitters
 * @param T     Period of one jitter
 * @param h     height (Values between 5 - 25) 
 */
void Otto::jitter(float steps, uint16_t T, int16_t h)
{
    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(25,h);
    int16_t A[_NBR_OF_SERVO]= {h, h, 0, 0};
    int16_t O[_NBR_OF_SERVO] = {0, 0, 0, 0};
    double phase_diff[_NBR_OF_SERVO] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Ascending & turn (Jitter while up&down)
 * 
 * @param steps Number of bends
 * @param T     Period of one bend
 * @param h     height (Values between 5 - 15)
 */
void Otto::ascendingTurn(float steps, uint16_t T, int16_t h)
{
    //-- Both feet and legs are 180 degrees out of phase
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(13,h);
    int16_t A[_NBR_OF_SERVO] = {h, h, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h+4, -h+4};
    double phase_diff[_NBR_OF_SERVO] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Moonwalker. Otto moves like Michael Jackson
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     Height. Typical valures between 15 and 40
 * @param dir   Direction: LEFT or RIGHT
 */
void Otto::moonwalker(float steps, uint16_t T, int16_t h, int dir)
{
    //-- This motion is similar to that of the caterpillar robots: A travelling
    //-- wave moving from one side to another
    //-- The two Otto's feet are equivalent to a minimal configuration. It is known
    //-- that 2 servos can move like a worm if they are 120 degrees out of phase
    //-- In the example of Otto, the two feet are mirrored so that we have:
    //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
    //--  is 60 degrees.
    //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
    //-   offset so that the robot tiptoe lightly
    
    int16_t A[_NBR_OF_SERVO] = {0, 0, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h/2+2, -h/2 -2};
    int phi = -dir * 90;
    double phase_diff[_NBR_OF_SERVO] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: rusaito. A mixture between moonwalker and walk
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     height (Values between 20 - 50)
 * @param dir   Direction: LEFT or RIGHT
 */
void Otto::crusaito(float steps, uint16_t T, int16_t h, int dir)
{
    int16_t A[_NBR_OF_SERVO]= {25, 25, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h/2+ 4, -h/2 - 4};
    double phase_diff[_NBR_OF_SERVO] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Flapping
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     height (Values between 10 - 30)
 * @param dir   direction: FOREWARD, BACKWARD
 */
void Otto::flapping(float steps, uint16_t T, int16_t h, int dir)
{
    int16_t A[_NBR_OF_SERVO]= {12, 12, h, h};
    int16_t O[_NBR_OF_SERVO] = {0, 0, h - 10, -h + 10};
    double phase_diff[_NBR_OF_SERVO] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief 
 * 
 * @param gesture 
 */
void Otto::playGesture(ottoGestures_t gesture){
    uint8_t gesturePOSITION[4];
  
    switch(gesture){

        case OttoHappy: 
            _tone(note_E5,50,30);
            putMouth(smile);
            sing(S_happy_short);
            swing(1,800,20); 
            sing(S_happy_short);

            home();
            putMouth(happyOpen);
        break;

        case OttoSuperHappy:
            putMouth(happyOpen);
            sing(S_happy);
            putMouth(happyClosed);
            tiptoeSwing(1,500,20);
            putMouth(happyOpen);
            sing(S_superHappy);
            putMouth(happyClosed);
            tiptoeSwing(1,500,20); 

            home();  
            putMouth(happyOpen);
        break;

        case OttoSad: 
            putMouth(sad);
            gesturePOSITION[0] = 110;//int sadPos[6]=      {110, 70, 20, 160};
            gesturePOSITION[1] = 70;
            gesturePOSITION[2] = 20;
            gesturePOSITION[3] = 160;
            _moveServos(700, gesturePOSITION);     
            bendTones(880, 830, 1.02, 20, 200);
            putMouth(sadClosed);
            bendTones(830, 790, 1.02, 20, 200);  
            putMouth(sadOpen);
            bendTones(790, 740, 1.02, 20, 200);
            putMouth(sadClosed);
            bendTones(740, 700, 1.02, 20, 200);
            putMouth(sadOpen);
            bendTones(700, 669, 1.02, 20, 200);
            putMouth(sad);
            delay(500);

            home();
            delay(300);
            putMouth(happyOpen);
        break;

        case OttoSleeping:
        gesturePOSITION[0] = 100;//int bedPos[6]=      {100, 80, 60, 120};
            gesturePOSITION[1] = 80;
            gesturePOSITION[2] = 60;
            gesturePOSITION[3] = 120;
            _moveServos(700, gesturePOSITION);     
            for(int i=0; i<4;i++){
            putAnimationMouth(dreamMouth,0);
            bendTones (100, 200, 1.04, 10, 10);
            putAnimationMouth(dreamMouth,1);
            bendTones (200, 300, 1.04, 10, 10);  
            putAnimationMouth(dreamMouth,2);
            bendTones (300, 500, 1.04, 10, 10);   
            delay(500);
            putAnimationMouth(dreamMouth,1);
            bendTones (400, 250, 1.04, 10, 1); 
            putAnimationMouth(dreamMouth,0);
            bendTones (250, 100, 1.04, 10, 1); 
            delay(500);
            } 

            putMouth(lineMouth);
            sing(S_cuddly);

            home();  
            putMouth(happyOpen);
        break;

        case OttoFart:
        gesturePOSITION[0] = 90;// int fartPos_1[6]=   {90, 90, 145, 122};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 145;
            gesturePOSITION[3] = 122;
            _moveServos(500,gesturePOSITION);
            delay(300);     
            putMouth(lineMouth);
            sing(S_fart1);  
            putMouth(tongueOut);
            delay(250);
            gesturePOSITION[0] = 90;// int fartPos_2[6]=   {90, 90, 80, 122};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 80;
            gesturePOSITION[3] = 122;
            _moveServos(500,gesturePOSITION);
            delay(300);
            putMouth(lineMouth);
            sing(S_fart2); 
            putMouth(tongueOut);
            delay(250);
            gesturePOSITION[0] = 90;// int fartPos_3[6]=   {90, 90, 145, 80};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 145;
            gesturePOSITION[3] = 80;
            _moveServos(500,gesturePOSITION);
            delay(300);
            putMouth(lineMouth);
            sing(S_fart3);
            putMouth(tongueOut);    
            delay(300);

            home(); 
            delay(500); 
            putMouth(happyOpen);
        break;

        case OttoConfused:
        gesturePOSITION[0] = 110;//int confusedPos[6]= {110, 70, 90, 90};
            gesturePOSITION[1] = 70;
            gesturePOSITION[2] = 90;
            gesturePOSITION[3] = 90;
            _moveServos(300, gesturePOSITION); 
            putMouth(confused);
            sing(S_confused);
            delay(500);

            home();  
            putMouth(happyOpen);
        break;

        case OttoLove:
            putMouth(heart);
            sing(S_cuddly);
            crusaito(2,1500,15,1);

            home(); 
            sing(S_happy_short);  
            putMouth(happyOpen);
        break;


        case OttoAngry: 
        gesturePOSITION[0] = 90;//int angryPos[6]=    {90, 90, 70, 110};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 70;
            gesturePOSITION[3] = 110;
            _moveServos(300, gesturePOSITION); 
            putMouth(angry);

            _tone(note_A5,100,30);
            bendTones(note_A5, note_D6, 1.02, 7, 4);
            bendTones(note_D6, note_G6, 1.02, 10, 1);
            bendTones(note_G6, note_A5, 1.02, 10, 1);
            delay(15);
            bendTones(note_A5, note_E5, 1.02, 20, 4);
            delay(400);
            gesturePOSITION[0] = 110;//int headLeft[6]=    {110, 110, 90, 90};
            gesturePOSITION[1] = 110;
            gesturePOSITION[2] = 90;
            gesturePOSITION[3] = 90;
            _moveServos(200, gesturePOSITION); 
            bendTones(note_A5, note_D6, 1.02, 20, 4);
            gesturePOSITION[0] = 70;//int headRight[6]=   {70, 70, 90, 90};
            gesturePOSITION[1] = 70;
            gesturePOSITION[2] = 90;
            gesturePOSITION[3] = 90;
            _moveServos(200, gesturePOSITION); 
            bendTones(note_A5, note_E5, 1.02, 20, 4);

            home();  
            putMouth(happyOpen);
        break;

        case OttoFretful: 
            putMouth(angry);
            bendTones(note_A5, note_D6, 1.02, 20, 4);
            bendTones(note_A5, note_E5, 1.02, 20, 4);
            delay(300);
            putMouth(lineMouth);

            for(int i=0; i<4; i++){
            gesturePOSITION[0] = 90;//int fretfulPos[6]=  {90, 90, 90, 110};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 90;
            gesturePOSITION[3] = 110;
            _moveServos(100, gesturePOSITION);   
            home();
            }

            putMouth(angry);
            delay(500);

            home();  
            putMouth(happyOpen);
        break;

        case OttoMagic:

            //Initial note frequency = 400
            //Final note frequency = 1000
            
            // Reproduce the animation four times
            for(int i = 0; i<4; i++){ 

            int noteM = 400; 

                for(int index = 0; index<6; index++){
                putAnimationMouth(adivinawi,index);
                bendTones(noteM, noteM+100, 1.04, 10, 10);    //400 -> 1000 
                noteM+=100;
                }

                clearMouth();
                bendTones(noteM-100, noteM+100, 1.04, 10, 10);  //900 -> 1100

                for(int index = 0; index<6; index++){
                putAnimationMouth(adivinawi,index);
                bendTones(noteM, noteM+100, 1.04, 10, 10);    //1000 -> 400 
                noteM-=100;
                }
            } 
    
            delay(300);
            putMouth(happyOpen);
        break;

        case OttoWave:
            
            // Reproduce the animation four times
            for(int i = 0; i<2; i++){ 

                int noteW = 500; 

                for(int index = 0; index<10; index++){
                putAnimationMouth(wave,index);
                bendTones(noteW, noteW+100, 1.02, 10, 10); 
                noteW+=101;
                }
                for(int index = 0; index<10; index++){
                putAnimationMouth(wave,index);
                bendTones(noteW, noteW+100, 1.02, 10, 10); 
                noteW+=101;
                }
                for(int index = 0; index<10; index++){
                putAnimationMouth(wave,index);
                bendTones(noteW, noteW-100, 1.02, 10, 10); 
                noteW-=101;
                }
                for(int index = 0; index<10; index++){
                putAnimationMouth(wave,index);
                bendTones(noteW, noteW-100, 1.02, 10, 10); 
                noteW-=101;
                }
            }    

            clearMouth();
            delay(100);
            putMouth(happyOpen);
        break;

        case OttoVictory:
            
            putMouth(smallSurprise);
            //final pos   = {90,90,150,30}
            for (int i = 0; i < 60; ++i){
            uint8_t pos[]={90,90,90+i,90-i};  
            _moveServos(10,pos);
            _tone(1600+i*20,15,1);
            }

            putMouth(bigSurprise);
            //final pos   = {90,90,90,90}
            for (int i = 0; i < 60; ++i){
            uint8_t pos[]={90,90,150-i,30+i};  
            _moveServos(10,pos);
            _tone(2800+i*20,15,1);
            }

            putMouth(happyOpen);
            //SUPER HAPPY
            //-----
            tiptoeSwing(1,500,20);
            sing(S_superHappy);
            putMouth(happyClosed);
            tiptoeSwing(1,500,20); 
            //-----

            home();
            clearMouth();
            putMouth(happyOpen);

        break;

        case OttoFail:
            putMouth(sadOpen);
            gesturePOSITION[0] = 90;//int bendPos_1[6]=   {90, 90, 70, 35};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 70;
            gesturePOSITION[3] = 35;
            _moveServos(300,gesturePOSITION);
            _tone(900,200,1);
            putMouth(sadClosed);
            gesturePOSITION[0] = 90;//int bendPos_2[6]=   {90, 90, 55, 35};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 55;
            gesturePOSITION[3] = 35;
            _moveServos(300,gesturePOSITION);
            _tone(600,200,1);
            putMouth(confused);
            gesturePOSITION[0] = 90;//int bendPos_3[6]=   {90, 90, 42, 35};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 42;
            gesturePOSITION[3] = 35;
            _moveServos(300,gesturePOSITION);
            _tone(300,200,1);
            gesturePOSITION[0] = 90;//int bendPos_4[6]=   {90, 90, 34, 35};
            gesturePOSITION[1] = 90;
            gesturePOSITION[2] = 34;
            gesturePOSITION[3] = 35;
            _moveServos(300,gesturePOSITION);
            putMouth(xMouth);

            detachServos();
            _tone(150,2200,1);
            
            delay(600);
            clearMouth();
            putMouth(happyOpen);
            home();

        break;

    }
} 
