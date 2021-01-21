/**
 * @file OttoLee.cpp
 * @author David LEVAL (dleval@senstronic.com)
 * @version 1.0
 * @date 2021-01-21
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

OttoLee::OttoLee(const uint8_t *pinServo, uint8_t pinBuzzer) : OttoSound(pinBuzzer) {
    _pinServo[LEG_L] = pinServo[LEG_L];
    _pinServo[LEG_R] = pinServo[LEG_R];
    _pinServo[FOOT_L] = pinServo[FOOT_L];
    _pinServo[FOOT_R] = pinServo[FOOT_R];
    _pinServo[ARM_L] = pinServo[ARM_L];
    _pinServo[ARM_R] = pinServo[ARM_R];
    _pinServo[HEAD] = pinServo[HEAD];
}


void OttoLee::init(bool load_calibration, uint8_t NoiseSensor, uint8_t USTrigger, uint8_t USEcho)
{
    attachServos();
    _isOttoResting=false;

    if (load_calibration) {
        for (uint8_t i = 0; i < 7; i++) {
        int servo_trim = EEPROM.read(i);
        if (servo_trim > 128) servo_trim -= 256;
        _servo[i].SetTrim(servo_trim);
        }
    }

    for (int i = 0; i < 7; i++) _servo_position[i] = 90;

    //US sensor init with the pins:
    _us.init(USTrigger, USEcho);


    //Buzzer & noise sensor pins: 
    _pinNoiseSensor = NoiseSensor;

    pinMode(NoiseSensor,INPUT);

}


///////////////////////////////////////////////////////////////////
//-- ATTACH & DETACH FUNCTIONS ----------------------------------//
///////////////////////////////////////////////////////////////////

/**
 * @brief 
 * 
 * @param pinServo 
 */
void OttoLee::attachServos()
{
    _servo[LEG_L].attach(_pinServo[LEG_L]);
    _servo[LEG_R].attach(_pinServo[LEG_R]);
    _servo[FOOT_L].attach(_pinServo[FOOT_L]);
    _servo[FOOT_R].attach(_pinServo[FOOT_R]);
    _servo[ARM_L].attach(_pinServo[ARM_L]);
    _servo[ARM_R].attach(_pinServo[ARM_R]);
    _servo[HEAD].attach(_pinServo[HEAD]);
}

/**
 * @brief 
 * 
 */
void OttoLee::detachServos()
{
    _servo[LEG_L].detach();
    _servo[LEG_R].detach();
    _servo[FOOT_L].detach();
    _servo[FOOT_R].detach();
    _servo[ARM_L].detach();
    _servo[ARM_R].detach();
    _servo[HEAD].detach();
}

///////////////////////////////////////////////////////////////////
//-- OSCILLATORS TRIMS ------------------------------------------//
///////////////////////////////////////////////////////////////////

/**
 * @brief 
 * 
 * @param LEG_L 
 * @param LEG_R 
 * @param FOOT_L 
 * @param FOOT_R 
 * @param ARM_L 
 * @param ARM_R 
 * @param HEAD 
 */
void OttoLee::setTrims(int8_t legLeft, int8_t legRight, int8_t footLeft, int8_t footRight, int8_t armLeft, int8_t armRight, int8_t head)
{ 
    _servo[LEG_L].SetTrim(legLeft);
    _servo[LEG_R].SetTrim(legRight);
    _servo[FOOT_L].SetTrim(footLeft);
    _servo[FOOT_R].SetTrim(footRight);
    _servo[ARM_L].SetTrim(armLeft);
    _servo[ARM_R].SetTrim(armRight);
    _servo[HEAD].SetTrim(head);
}

/**
 * @brief 
 * 
 */
void OttoLee::saveTrimsOnEEPROM() 
{
    for (int i = 0; i < 4; i++){ 
        EEPROM.write(i, _servo[i].getTrim());
    } 
}

///////////////////////////////////////////////////////////////////
//-- BASIC MOTION FUNCTIONS -------------------------------------//
///////////////////////////////////////////////////////////////////

/**
 * @brief 
 * 
 * @param time 
 * @param servo_target 
 */
void OttoLee::_moveServos(int time, int  servo_target[]) 
{
    attachServos();
    if(getRestState()==true){
        setRestState(false);
    }

    if(time>10){
        for (int i = 0; i < 7; i++) _increment[i] = ((servo_target[i]) - _servo_position[i]) / (time / 10.0);
        _final_time =  millis() + time;

        for (int iteration = 1; millis() < _final_time; iteration++) {
        _partial_time = millis() + 10;
        for (int i = 0; i < 7; i++) _servo[i].SetPosition(_servo_position[i] + (iteration * _increment[i]));
        while (millis() < _partial_time); //pause
        }
    }
    else{
        for (int i = 0; i < 7; i++) _servo[i].SetPosition(servo_target[i]);
    }
    for (int i = 0; i < 7; i++) _servo_position[i] = servo_target[i];
}

/**
 * @brief 
 * 
 * @param position 
 * @param servo_number 
 */
void OttoLee::_moveSingle(int position, int servo_number) 
{
    if (position > 180) position = 90;
    if (position < 0) position = 90;
    attachServos();
    if(getRestState()==true) {
        setRestState(false);
    }
    int servoNumber = servo_number;
    if (servoNumber == 0) {
        _servo[0].SetPosition(position);
    }
    if (servoNumber == 1) {
        _servo[1].SetPosition(position);
    }
    if (servoNumber == 2) {
        _servo[2].SetPosition(position);
    }
    if (servoNumber == 3) {
        _servo[3].SetPosition(position);
    }
    if (servoNumber == 4) {
        _servo[4].SetPosition(position);
    }
    if (servoNumber == 5) {
        _servo[5].SetPosition(position);
    }
    if (servoNumber == 6) {
        _servo[6].SetPosition(position);
    }
}


void OttoLee::oscillateServos(int A[7], int O[7], int T, double phase_diff[7], float cycle)
{

  for (int i=0; i<7; i++) {
    _servo[i].SetO(O[i]);
    _servo[i].SetA(A[i]);
    _servo[i].SetT(T);
    _servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()){
     for (int i=0; i<7; i++){
        _servo[i].refresh();
     }
  }
}


void OttoLee::_execute(int A[7], int O[7], int T, double phase_diff[7], float steps)
{
    attachServos();
    if(getRestState()==true){
        setRestState(false);
    }

    int cycles=(int)steps;    

    //-- Execute complete cycles
    if (cycles >= 1) {
        for(int i = 0; i < cycles; i++) {
            oscillateServos(A,O, T, phase_diff);
        }
    }
        
    //-- Execute the final not complete cycle    
    oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}

///////////////////////////////////////////////////////////////////
//-- HOME = Otto at rest position -------------------------------//
///////////////////////////////////////////////////////////////////
void OttoLee::home(){

  if(_isOttoResting == false){ //Go to rest position only if necessary

    int homes[7] = {90, 90, 90, 90, 90, 90, 90}; //All the servos at rest position
    _moveServos(500,homes);   //Move the servos in half a second

    detachServos();
    _isOttoResting = true;
  }
}

bool OttoLee::getRestState(){

    return _isOttoResting;
}

void OttoLee::setRestState(bool state){

    _isOttoResting = state;
}

///////////////////////////////////////////////////////////////////
//-- PREDETERMINED MOTION SEQUENCES -----------------------------//
///////////////////////////////////////////////////////////////////

/**
 * @brief Otto predetermined movement: Jump
 * 
 * @param steps Number of steps
 * @param T Period
 */
void OttoLee::jump(float steps, int T){

    int up[7]={90,90,150,30,110,70,90};
    _moveServos(T,up);
    int down[7]={90,90,90,90,90,90,90};
    _moveServos(T,down);
}

/**
 * @brief Otto predetermined movement: Walking  (forward or backward) 
 * 
 * @param steps Number of steps
 * @param T Period
 * @param dir Direction: FORWARD / BACKWARD
 */
void OttoLee::walk(float steps, int T, int dir, int armOsc, int headOsc)
{
    //-- Oscillator parameters for walking
    //-- Hip sevos are in phase
    //-- Feet servos are in phase
    //-- Hip and feet are 90 degrees out of phase
    //--      -90 : Walk forward
    //--       90 : Walk backward
    //-- Feet servos also have the same offset (for tiptoe a little bit)
    int A[7]= {30, 30, 20, 20, armOsc, armOsc, headOsc};
    int O[7] = {0, 0, 4, -4,0 ,0, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90), 0, 0, 0};

    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps);  
}

/**
 * @brief Otto predetermined movement: Turning (left or right)
 * 
 * @param steps Number of steps
 * @param T Period
 * @param dir Direction: LEFT / RIGHT
 */
void OttoLee::turn(float steps, int T, int dir, int armOsc, int headOsc)
{
    //-- Same coordination than for walking (see Otto::walk)
    //-- The Amplitudes of the hip's oscillators are not igual
    //-- When the right hip servo amplitude is higher, the steps taken by
    //--   the right leg are bigger than the left. So, the robot describes an 
    //--   left arc
    int A[7]= {30, 30, 20, 20, armOsc, armOsc, headOsc};
    int O[7] = {0, 0, 4, -4, 0, 0, 0};
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
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Lateral bend
 * 
 * @param steps Number of bends
 * @param T Period of one bend
 * @param dir RIGHT=Right bend LEFT=Left bend
 */
void OttoLee::bend(int steps, int T, int dir)
{
    //Parameters of all the movements. Default: Left bend
    int bend1[7]={90, 90, 62, 35, 120, 60, 90}; 
    int bend2[7]={90, 90, 62, 105, 60, 120, 90};
    int homes[7]={90, 90, 90, 90, 90, 90, 90};

    //Time of one bend, constrained in order to avoid movements too fast.
    //T=max(T, 600);

    //Changes in the parameters if right direction is chosen 
    if(dir==-1)
    {
        bend1[2]=180-35;
        bend1[3]=180-60;  //Not 65. Otto is unbalanced
        bend2[2]=180-105;
        bend2[3]=180-60;
    }

    //Time of the bend movement. Fixed parameter to avoid falls
    int T2=800; 

    //Bend movement
    for (int i=0;i<steps;i++)
    {
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
 * @param T Period of one shake
 * @param dir RIGHT=Right leg LEFT=Left leg
 */
void OttoLee::shakeLeg(int steps,int T,int dir){

    //This variable change the amount of shakes
    int numberLegMoves=2;

    //Parameters of all the movements. Default: Right leg
    int shake_leg1[7]={90, 90, 58, 35, 90, 90, 90};   
    int shake_leg2[7]={90, 90, 58, 120, 100, 80, 90};
    int shake_leg3[7]={90, 90, 58, 60, 80, 100, 90};
    int homes[7]={90, 90, 90, 90, 90, 90, 90};

    //Changes in the parameters if left leg is chosen
    if(dir==-1)      
    {
        shake_leg1[2]=180-35;
        shake_leg1[3]=180-58;
        shake_leg2[2]=180-120;
        shake_leg2[3]=180-58;
        shake_leg3[2]=180-60;
        shake_leg3[3]=180-58;
    }
  
    //Time of the bend movement. Fixed parameter to avoid falls
    int T2=2000;    
    //Time of one shake, constrained in order to avoid movements too fast.            
    T=T-T2;
    T=max(T,200*numberLegMoves);  

    for (int j=0; j<steps;j++)
    {
    //Bend movement
    _moveServos(T2/2,shake_leg1);
    _moveServos(T2/2,shake_leg2);
    
        //Shake movement
        for (int i=0;i<numberLegMoves;i++)
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
 * @param T Period
 * @param h Jump height: SMALL / MEDIUM / BIG (or a number in degrees 0 - 90)
 */
void OttoLee::updown(float steps, int T, int h)
{
    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    int A[7]= {0, 0, h, h, h, h, 0};
    int O[7] = {0, 0, h, -h, h, -h, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(-90), DEG2RAD(90),DEG2RAD(-90), DEG2RAD(90) , 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Hands up
 * 
 */
void OttoLee::handsup()
{
    int homes[7]={90, 90, 90, 90, 20, 160, 90}; //
    _moveServos(500,homes);   //Move the servos in half a second
}

/**
 * @brief Otto predetermined movement: Hand Wave , either left or right
 * 
 * @param dir 
 */
void OttoLee::handwave(int dir)
{
    if(dir==-1)      
    {
        int A[7]= {0, 0, 0, 0, 30, 0, 0}; // left hand wave
        int O[7] = {0, 0, 0, 0, -30, -40, 0};
        double phase_diff[7] = {0, 0, 0, 0, DEG2RAD(0), 0, 0};
            //-- Let's oscillate the servos!
        _execute(A, O, 500, phase_diff, 5); 
    }
    if(dir==1)      
    {
        int A[7]= {0, 0, 0, 0, 0, 30, 0}; // right hand wave
        int O[7] = {0, 0, 0, 0, 40, 60, 0};
        double phase_diff[7] = {0, 0, 0, 0, 0, DEG2RAD(0), 0};
            //-- Let's oscillate the servos!
        _execute(A, O, 500, phase_diff, 1); 
    }  
}

/**
 * @brief Otto predetermined movement: swinging side to side
 * 
 * @param steps Number of steps
 * @param T Period
 * @param h Amount of swing (from 0 to 50 aprox)
 */
void OttoLee::swing(float steps, int T, int h)
{
    //-- Both feets are in phase. The offset is half the amplitude
    //-- It causes the robot to swing from side to side
    int A[7]= {0, 0, h, h, h, h, 0};
    int O[7] = {0, 0, h/2, -h/2, h, -h, 0};
    double phase_diff[7] = {0, 0, DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: swinging side to side without touching the floor with the heel
 * 
 * @param steps Number of steps
 * @param T Period
 * @param h Amount of swing (from 0 to 50 aprox)
 */
void OttoLee::tiptoeSwing(float steps, int T, int h){

    //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
    //-- It causes the robot to swing from side to side
    int A[7]= {0, 0, h, h, h, h, 0};
    int O[7] = {0, 0, h, -h, h, -h, 0};
    double phase_diff[7] = {0, 0, 0, 0, 0, 0, 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Jitter
 * 
 * @param steps Number of jitters
 * @param T Period of one jitter
 * @param h height (Values between 5 - 25) 
 */
void OttoLee::jitter(float steps, int T, int h){

    //-- Both feet are 180 degrees out of phase
    //-- Feet amplitude and offset are the same
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(25,h);
    int A[7]= {h, h, 0, 0, 0, 0, 0};
    int O[7] = {0, 0, 0, 0, 0, 0, 0};
    double phase_diff[7] = {DEG2RAD(-90), DEG2RAD(90), 0, 0, 0, 0, 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Ascending & turn (Jitter while up&down)
 * 
 * @param steps Number of bends
 * @param T Period of one bend
 * @param h height (Values between 5 - 15)
 */
void OttoLee::ascendingTurn(float steps, int T, int h)
{
    //-- Both feet and legs are 180 degrees out of phase
    //-- Initial phase for the right foot is -90, so that it starts
    //--   in one extreme position (not in the middle)
    //-- h is constrained to avoid hit the feets
    h = min(13,h);
    int A[7] = {h, h, h, h, 40, 40, 0};
    int O[7] = {0, 0, h+4, -h+4,0,0, 0};
    double phase_diff[7] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90), 0, 0, 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Moonwalker. Otto moves like Michael Jackson
 * 
 * @param steps Number of steps
 * @param T Period
 * @param h Height. Typical valures between 15 and 40
 * @param dir Direction: LEFT / RIGHT
 */
void OttoLee::moonwalker(float steps, int T, int h, int dir)
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
    
    int A[7] = {0, 0, h, h, h, h, 0};
    int O[7] = {0, 0, h/2+2, -h/2 -2, -h, h, 0};
    int phi = -dir * 90;
    double phase_diff[7] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi), DEG2RAD(phi), DEG2RAD(phi), 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: rusaito. A mixture between moonwalker and walk
 * 
 * @param steps Number of steps
 * @param T Period
 * @param h height (Values between 20 - 50)
 * @param dir Direction: LEFT / RIGHT
 */
void OttoLee::crusaito(float steps, int T, int h, int dir)
{
    int A[7]= {25, 25, h, h, 0, 0, 0};
    int O[7] = {0, 0, h/2+ 4, -h/2 - 4, 0, 0, 0};
    double phase_diff[7] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir), 0, 0, 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief Otto predetermined movement: Flapping
 * 
 * @param steps Number of steps
 * @param T Period
 * @param h height (Values between 10 - 30)
 * @param dir direction: FOREWARD, BACKWARD
 */
void OttoLee::flapping(float steps, int T, int h, int dir)
{
    int A[7]= {12, 12, h, h, 0, 0, 0};
    int O[7] = {0, 0, h - 10, -h + 10, 0, 0, 0};
    double phase_diff[7] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir), 0, 0, 0};
    
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

/**
 * @brief 
 * 
 */
void OttoLee::headNo(float steps, int T)
{
    int A[7]= {0, 0, 0, 0, 0, 0, 30}; // right hand wave
    int O[7] = {0, 0, 0, 0, 0, 0, 0};
    double phase_diff[7] = {0, 0, 0, 0, 0, 0, DEG2RAD(0)};
    //-- Let's oscillate the servos!
    _execute(A, O, T, phase_diff, steps); 
}

///////////////////////////////////////////////////////////////////
//-- SENSORS FUNCTIONS  -----------------------------------------//
///////////////////////////////////////////////////////////////////

//---------------------------------------------------------
//-- Otto getDistance: return Otto's ultrasonic sensor measure
//---------------------------------------------------------

/**
 * @brief return Otto's ultrasonic sensor measure
 * 
 * @return float distance measurement
 */
float OttoLee::getDistance()
{
    return _us.read();
}

/**
 * @brief return Otto's noise sensor measure
 * 
 * @return int noise measurement
 */
int OttoLee::getNoise()
{
    int noiseLevel = 0;
    int noiseReadings = 0;
    int numReadings = 2;  

    noiseLevel = analogRead(_pinNoiseSensor);

    for(int i=0; i<numReadings; i++){
        noiseReadings += analogRead(_pinNoiseSensor);
        delay(4); // delay in between reads for stability
    }

    noiseLevel = noiseReadings / numReadings;

    return noiseLevel;
}
