/**
 * @file OttoLee.cpp
 * @author David LEVAL (dleval@senstronic.com)
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
#include <OttoLee.h>

/**
 * @brief Construct a new Otto Lee:: Otto Lee object
 * 
 * @param legLeft           Left leg pin Servo
 * @param legRight          Right leg pin Servo
 * @param footLeft          Left foot pin Servo
 * @param footRight         Right foot pin Servo
 * @param armLeft           Left arm pin Servo
 * @param armRight          Right arm pin Servo
 * @param head              Head pin Servo
 * @param pinNoiseSensor 
 * @param pinBuzzer 
 * @param pinUSTrigger 
 * @param pinUSEcho 
 */
OttoLee::OttoLee(uint8_t legLeft, uint8_t legRight, uint8_t footLeft, uint8_t footRight, uint8_t armLeft, uint8_t armRight, uint8_t head, uint8_t pinNoiseSensor, uint8_t pinBuzzer, uint8_t pinUSTrigger, uint8_t pinUSEcho) : 
    OttoSound(pinBuzzer), OttoSensor(pinNoiseSensor), OttoServo(legLeft, legRight, footLeft, footRight, armLeft, armRight, head)
{
    OttoSensor::init(pinUSTrigger, pinUSEcho);
}

/**
 * @brief 
 * 
 * @param load_calibration 
 */
void OttoLee::init(bool load_calibration)
{
    OttoServo::init(load_calibration);
}


/** Predetermined motion sequences ********************************************/

/**
 * @brief Otto predetermined movement: Jump
 * 
 * @param steps Number of steps
 * @param T     Period
 */
void OttoLee::jump(float steps, uint16_t T)
{
    uint8_t up[7]={90,90,150,30,110,70,90};
    moveServos(T,up);
    uint8_t down[7]={90,90,90,90,90,90,90};
    moveServos(T,down);
}

/**
 * @brief Otto predetermined movement: Walking  (forward or backward) 
 * 
 * @param steps Number of steps
 * @param T Period
 * @param dir Direction: FORWARD / BACKWARD
 */
void OttoLee::walk(float steps, uint16_t T, int8_t dir, uint16_t armOsc, uint16_t headOsc)
{
    //-- Oscillator parameters for walking
    //-- Hip sevos are in phase
    //-- Feet servos are in phase
    //-- Hip and feet are 90 degrees out of phase
    //--      -90 : Walk forward
    //--       90 : Walk backward
    //-- Feet servos also have the same offset (for tiptoe a little bit)

    uint16_t A[7]= {30, 30, 20, 20, armOsc, armOsc, headOsc};
    uint16_t O[7] = {0, 0, 4, 4, 0 ,0 , 0};
    double phase_diff[7] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90), 0, 0, 0};

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
void OttoLee::turn(float steps, uint16_t T, int8_t dir, uint16_t armOsc, uint16_t headOsc)
{
    //-- Same coordination than for walking (see Otto::walk)
    //-- The Amplitudes of the hip's oscillators are not igual
    //-- When the right hip servo amplitude is higher, the steps taken by
    //--   the right leg are bigger than the left. So, the robot describes an 
    //--   left arc
    uint16_t A[7]= {30, 30, 20, 20, armOsc, armOsc, headOsc};
    uint16_t O[7] = {0, 0, 4, 4, 0, 0, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(-90), DEG2RAD(-90), 0, 0, 0}; 
        
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
void OttoLee::bend(uint16_t steps, uint16_t T, int8_t dir)
{
    //Parameters of all the movements. Default: Left bend
    uint8_t bend1[7]={90, 90, 62, 35, 120, 60, 90}; 
    uint8_t bend2[7]={90, 90, 62, 105, 60, 120, 90};
    uint8_t homes[7]={90, 90, 90, 90, 90, 90, 90};

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
        moveServos(T2/2,bend1);
        moveServos(T2/2,bend2);
        delay(T*0.8);
        moveServos(500,homes);
    }
}


/**
 * @brief Otto predetermined movement: Shake a leg
 * 
 * @param steps Number of shakes
 * @param T     Period of one shake
 * @param dir   RIGHT=Right leg LEFT=Left leg
 */
void OttoLee::shakeLeg(uint16_t steps, uint16_t T, int8_t dir)
{
    //This variable change the amount of shakes
    uint16_t numberLegMoves=2;

    //Parameters of all the movements. Default: Right leg
    uint8_t shake_leg1[7]={90, 90, 58, 35, 90, 90, 90};   
    uint8_t shake_leg2[7]={90, 90, 58, 120, 100, 80, 90};
    uint8_t shake_leg3[7]={90, 90, 58, 60, 80, 100, 90};
    uint8_t homes[7]={90, 90, 90, 90, 90, 90, 90};

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
        moveServos(T2/2,shake_leg1);
        moveServos(T2/2,shake_leg2);
        //Shake movement
        for (uint16_t i=0;i<numberLegMoves;i++)
        {
        moveServos(T/(2*numberLegMoves),shake_leg3);
        moveServos(T/(2*numberLegMoves),shake_leg2);
        }
        moveServos(500,homes); //Return to home position
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
void OttoLee::updown(float steps, uint16_t T, uint16_t h)
{
    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    uint16_t A[7]= {0, 0, h, h, h, h, 0};
    uint16_t O[7] = {0, 0, h, -h, h, -h, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(-90), DEG2RAD(90),DEG2RAD(-90), DEG2RAD(90) , 0};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Hands up
 * 
 */
void OttoLee::handsup()
{
    uint8_t homes[7]={90, 90, 90, 90, 20, 160, 90}; //
    moveServos(500,homes);   //Move the servos in half a second
}

/**
 * @brief Otto predetermined movement: Hand Wave , either left or right
 * 
 * @param dir direction (-1 or 1)
 */
void OttoLee::handwave(int8_t dir)
{
    if(dir==-1) {
        uint16_t A[7]= {0, 0, 0, 0, 30, 0, 0}; // left hand wave
        uint16_t O[7] = {0, 0, 0, 0, 30, 40, 0};
        double phase_diff[7] = {0, 0, 0, 0, DEG2RAD(0), 0, 0};
            //-- Let's oscillate the servos!
        execute(A, O, 500, phase_diff, 5); 
    }
    if(dir==1) {
        uint16_t A[7]= {0, 0, 0, 0, 0, 30, 0}; // right hand wave
        uint16_t O[7] = {0, 0, 0, 0, 40, 60, 0};
        double phase_diff[7] = {0, 0, 0, 0, 0, DEG2RAD(0), 0};
            //-- Let's oscillate the servos!
        execute(A, O, 500, phase_diff, 1); 
    }  
}

/**
 * @brief Otto predetermined movement: swinging side to side
 * 
 * @param steps Number of steps
 * @param T     Period
 * @param h     Amount of swing (from 0 to 50 aprox)
 */
void OttoLee::swing(float steps, uint16_t T, uint16_t h)
{
    //-- Both feets are in phase. The offset is half the amplitude
    //-- It causes the robot to swing from side to side
    uint16_t A[7]= {0, 0, h, h, h, h, 0};
    uint16_t O[7] = {0, 0, h/2, -h/2, h, -h, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), 0};
    
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
void OttoLee::tiptoeSwing(float steps, uint16_t T, uint16_t h){

    //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
    //-- It causes the robot to swing from side to side
    uint16_t A[7]= {0, 0, h, h, h, h, 0};
    uint16_t O[7] = {0, 0, h, -h, h, -h, 0};
    double phase_diff[7] = {0, 0, 0, 0, 0, 0, 0};
    
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
void OttoLee::jitter(float steps, uint16_t T, uint16_t h)
{
    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(25,h);
    uint16_t A[7]= {h, h, 0, 0, 0, 0, 0};
    uint16_t O[7] = {0, 0, 0, 0, 0, 0, 0};
    double phase_diff[7] = {DEG2RAD(-90), DEG2RAD(90), 0, 0, 0, 0, 0};
    
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
void OttoLee::ascendingTurn(float steps, uint16_t T, uint16_t h)
{
    //-- Both feet and legs are 180 degrees out of phase
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(13,h);
    uint16_t A[7] = {h, h, h, h, 40, 40, 0};
    uint16_t O[7] = {0, 0, h+4, -h+4,0,0, 0};
    double phase_diff[7] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90), 0, 0, 0};
    
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
void OttoLee::moonwalker(float steps, uint16_t T, uint16_t h, int dir)
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
    
    uint16_t A[7] = {0, 0, h, h, h, h, 0};
    uint16_t O[7] = {0, 0, h/2+2, -h/2 -2, -h, h, 0};
    int phi = -dir * 90;
    double phase_diff[7] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi), DEG2RAD(phi), DEG2RAD(phi), 0};
    
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
void OttoLee::crusaito(float steps, uint16_t T, uint16_t h, int dir)
{
    uint16_t A[7]= {25, 25, h, h, 0, 0, 0};
    uint16_t O[7] = {0, 0, h/2+ 4, -h/2 - 4, 0, 0, 0};
    double phase_diff[7] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir), 0, 0, 0};
    
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
void OttoLee::flapping(float steps, uint16_t T, uint16_t h, int dir)
{
    uint16_t A[7]= {12, 12, h, h, 0, 0, 0};
    uint16_t O[7] = {0, 0, h - 10, -h + 10, 0, 0, 0};
    double phase_diff[7] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir), 0, 0, 0};
    
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Head No
 * 
 * @param steps Number of steps
 * @param T     Period
 */
void OttoLee::headNo(float steps, uint16_t T)
{
    uint16_t A[7]= {0, 0, 0, 0, 0, 0, 30}; // right hand wave
    uint16_t O[7] = {0, 0, 0, 0, 0, 0, 0};
    double phase_diff[7] = {0, 0, 0, 0, 0, 0, DEG2RAD(0)};
    //-- Let's oscillate the servos!
    execute(A, O, T, phase_diff, steps); 
}

