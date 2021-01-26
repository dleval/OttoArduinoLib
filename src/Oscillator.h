//--------------------------------------------------------------
//-- Oscillator.pde
//-- Generate sinusoidal oscillations in the servos
//--------------------------------------------------------------
//-- (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
//-- GPL license
//--------------------------------------------------------------
#ifndef Oscillator_h
#define Oscillator_h

#include <stdint.h>
#include <Servo.h>

//-- Macro for converting from degrees to radians
#ifndef DEG2RAD
  #define DEG2RAD(g) ((g)*M_PI)/180
#endif

class Oscillator
{
  public:
    Oscillator(int trim=0) {_trim=trim;};
    void attach(int pin, bool rev =false);
    void detach();
    
    void SetA(uint16_t A) {_A=A;};
    void SetO(uint16_t O) {_O=O;};
    void SetPh(double Ph) {_phase0=Ph;};
    void SetT(uint16_t T);
    void SetTrim(int8_t trim){_trim=trim;};
    int8_t getTrim() {return _trim;};
    void SetPosition(uint8_t position); 
    void Stop() {_stop=true;};
    void Play() {_stop=false;};
    void Reset() {_phase=0;};
    void refresh();
    
  private:
    bool next_sample();  
    
  private:
    //-- Servo that is attached to the oscillator
    Servo _servo;
    
    //-- Oscillators parameters
    uint16_t _A;  //-- Amplitude (degrees)
    uint16_t _O;  //-- Offset (degrees)
    uint16_t _T;  //-- Period (miliseconds)
    double _phase0;   //-- Phase (radians)
    
    //-- Internal variables
    int16_t _pos;         //-- Current servo pos
    int8_t _trim;        //-- Calibration offset
    double _phase;    //-- Current phase
    double _inc;      //-- Increment of phase
    double _N;        //-- Number of samples
    uint16_t _TS; //-- sampling period (ms)
    
    uint32_t _previousMillis; 
    uint32_t _currentMillis;
    
    //-- Oscillation mode. If true, the servo is stopped
    bool _stop;

    //-- Reverse mode
    bool _rev;
};

#endif