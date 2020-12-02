#include <Arduino.h>
#include <OttoLee.h>

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
 * @param load_calibration 
 * @param NoiseSensor 
 * @param Buzzer 
 * @param USTrigger 
 * @param USEcho 
 */
void OttoLee::init(uint8_t LEG_L, uint8_t LEG_R, uint8_t FOOT_L, uint8_t FOOT_R, uint8_t ARM_L, uint8_t ARM_R, uint8_t HEAD, bool load_calibration, uint8_t NoiseSensor, uint8_t Buzzer, uint8_t USTrigger, uint8_t USEcho)
{
    _servo_pins[0] = LEG_L;
    _servo_pins[1] = LEG_R;
    _servo_pins[2] = FOOT_L;
    _servo_pins[3] = FOOT_R;
    _servo_pins[4] = ARM_L;
    _servo_pins[5] = ARM_R;
    _servo_pins[6] = HEAD;

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
    _pinBuzzer = Buzzer;
    _pinNoiseSensor = NoiseSensor;

    pinMode(Buzzer,OUTPUT);
    pinMode(NoiseSensor,INPUT);

}


///////////////////////////////////////////////////////////////////
//-- ATTACH & DETACH FUNCTIONS ----------------------------------//
///////////////////////////////////////////////////////////////////

/**
 * @brief 
 * 
 */
void OttoLee::attachServos()
{
    _servo[0].attach(_servo_pins[0]);
    _servo[1].attach(_servo_pins[1]);
    _servo[2].attach(_servo_pins[2]);
    _servo[3].attach(_servo_pins[3]);
    _servo[4].attach(_servo_pins[4]);
    _servo[5].attach(_servo_pins[5]);
    _servo[6].attach(_servo_pins[6]);
}

/**
 * @brief 
 * 
 */
void OttoLee::detachServos()
{
    _servo[0].detach();
    _servo[1].detach();
    _servo[2].detach();
    _servo[3].detach();
    _servo[4].detach();
    _servo[5].detach();
    _servo[6].detach();
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
void OttoLee::setTrims(int8_t LEG_L, int8_t LEG_R, int8_t FOOT_L, int8_t FOOT_R, int8_t ARM_L, int8_t ARM_R, int8_t HEAD)
{
    _servo[0].SetTrim(LEG_L);
    _servo[1].SetTrim(LEG_R);
    _servo[2].SetTrim(FOOT_L);
    _servo[3].SetTrim(FOOT_R);
    _servo[4].SetTrim(ARM_L);
    _servo[5].SetTrim(ARM_R);
    _servo[6].SetTrim(HEAD);
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


void OttoLee::oscillateServos(int A[7], int O[7], int T, double phase_diff[7], float cycle=1)
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


void OttoLee::_execute(int A[7], int O[7], int T, double phase_diff[7], float steps = 1.0)
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


///////////////////////////////////////////////////////////////////
//-- SOUNDS -----------------------------------------------------//
///////////////////////////////////////////////////////////////////

void OttoLee::_tone (float noteFrequency, long noteDuration, int silentDuration)
{
    // tone(10,261,500);
    // delay(500);

    if(silentDuration==0){silentDuration=1;}

    TimerFreeTone(OttoLee::_pinBuzzer, noteFrequency, noteDuration);
    //delay(noteDuration);       //REMOVED FOR TimerFreeTone, PUT BACK for TONE       milliseconds to microseconds
    //noTone(PIN_Buzzer);
      
    delay(silentDuration);     //
}

void OttoLee::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration)
{
    //Examples:
    //  bendTones (880, 2093, 1.02, 18, 1);
    //  bendTones (note_A5, note_C7, 1.02, 18, 0);

    if(silentDuration==0){silentDuration=1;}

    if(initFrequency < finalFrequency)
    {
        for (int i=initFrequency; i<finalFrequency; i=i*prop) {
            _tone(i, noteDuration, silentDuration);
        }

    } else{

        for (int i=initFrequency; i>finalFrequency; i=i/prop) {
            _tone(i, noteDuration, silentDuration);
        }
    }
}


void OttoLee::sing(int songName){
  switch(songName){

    case S_connection:
      _tone(note_E5,50,30);
      _tone(note_E6,55,25);
      _tone(note_A6,60,10);
    break;

    case S_disconnection:
      _tone(note_E5,50,30);
      _tone(note_A6,55,25);
      _tone(note_E6,50,10);
    break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
    break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
    break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
    break;

    case S_mode3:
      _tone(note_E6,50,100); //D6
      _tone(note_G6,50,80);  //E6
      _tone(note_D7,300,0);  //G6
    break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
    break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
           _tone(note_B5,5,10);
      }
    break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          _tone(note_C6,10,10);
      }
    break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
    break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
    break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
    break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
    break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
    break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
    break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2); 
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
    break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
    break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
    break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
    break;

  }
}


