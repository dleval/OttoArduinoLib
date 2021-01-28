/**
 * @file OttoSound.cpp
 * @author David LEVAL (dleval@dle-dev.com)
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
#include "OttoSoundSong.h"
#include "OttoSound.h"

/**
 * @brief Construct a new OttoSound::OttoSound object
 * 
 * @param pinBuzzer : Buzzer control pin
 */
OttoSound::OttoSound(uint8_t pinBuzzer)
{
    _pinBuzzer = pinBuzzer;
    pinMode(_pinBuzzer, OUTPUT);
}

/**
 * @brief Destroy the OttoSound::OttoSound object
 * 
 */
OttoSound::~OttoSound()
{
}

/**
 * @brief Play a note on buzzer pin.
 * 
 * @param noteFrequency     : Note frequency (Hz)
 * @param noteDuration      : Note duration (ms)
 * @param silentDuration    : Duration of silence after note (ms)
 * @param volume            : volume control (1 to 10)
 */
void OttoSound::_tone (float noteFrequency, uint32_t noteDuration, uint16_t silentDuration, uint8_t volume)
{
    const uint8_t _tftVolume[] = { 255, 200, 150, 125, 100, 87, 50, 33, 22, 2 }; // Duty for linear volume control.

    if(noteFrequency == 0 || volume == 0) {
        //Wait for the duration of the note if frequency or volume are zero
		delay(noteDuration);
	} else {
#ifdef __USE_ARDUINO_TONE_LIB
        tone(_pinBuzzer,(uint16_t)noteFrequency, noteDuration);
#else
        uint32_t period = (uint32_t)(1000000 / noteFrequency); //Calculate the square wave length (in microseconds).
        uint32_t duty = period / _tftVolume[min(volume, 10) - 1]; // Calculate the duty cycle (volume).
        uint32_t startTime = millis();
        while(millis() < (startTime + noteDuration)) {
            digitalWrite(_pinBuzzer,HIGH);  // Set pin high.
            delayMicroseconds(duty); // Square wave duration (how long to leave pin high).
            digitalWrite(_pinBuzzer,LOW);   // Set pin low.
            delayMicroseconds(period - duty); // Square wave duration (how long to leave pin low).
        }
#endif //__USE_ARDUINO_TONE_LIB
    }

#ifdef __USE_ARDUINO_TONE_LIB
    noTone(_pinBuzzer);
#else
    digitalWrite(_pinBuzzer, 0);
#endif //__USE_ARDUINO_TONE_LIB
      
    if(silentDuration) delay(silentDuration);  
}

/**
 * @brief Play a note with a bend effect.
 * 
 * @param initFrequency     : Initial note frequency (Hz)
 * @param finalFrequency    : Final note frequency (Hz)
 * @param prop              : Multiplying coefficient of frequency increase
 * @param noteDuration      : Note duration at each frequency (ms)
 * @param silentDuration    : Duration of silence after each note (ms)
 * 
 * @example 
 *      bendTones (880, 2093, 1.02, 18, 1);
 *      bendTones (note_A5, note_C7, 1.02, 18, 0);
 */
void OttoSound::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration)
{
    if(initFrequency < finalFrequency) {
        for (float freq=initFrequency; freq<finalFrequency; freq=(freq*prop)) _tone(freq, noteDuration, silentDuration);
    } else {
        for (float freq=initFrequency; freq>finalFrequency; freq=(freq/prop)) _tone(freq, noteDuration, silentDuration);
    }
}

/**
 * @brief Play a defined sequence of notes
 * 
 * @param songName 
 */
void OttoSound::sing(uint8_t songName)
{
    switch(songName) 
    {
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

        case SongSilentNight:
            songSilentNight();
            break;
    }
}

/**
 * @brief 
 * 
 */
void OttoSound::r2d2()
{
    switch (random(1,7)) {
        case 1: _r2d2Phrase1(); break;
        case 2: _r2d2Phrase2(); break;
        case 3: _r2d2Phrase1(); _r2d2Phrase2(); break;
        case 4: _r2d2Phrase1(); _r2d2Phrase2(); _r2d2Phrase1(); break;
        case 5: _r2d2Phrase1(); _r2d2Phrase2(); _r2d2Phrase1(); _r2d2Phrase2(); _r2d2Phrase1(); break;
        case 6: _r2d2Phrase2(); _r2d2Phrase1(); _r2d2Phrase2(); break;
    }
    for (int i=0; i<=random(3, 9); i++) {
        float freq = random(300, 4000);
        // tone(_pinBuzzer, freq);          
        // delay(random(70, 170));           
        // noTone(_pinBuzzer);         
        // delay(random(0, 30)); 
        _tone(freq,random(70, 170),random(0, 30));
    } 
    noTone(_pinBuzzer); 
    delay(2);
}

/**
 * @brief 
 * 
 */
void OttoSound::_r2d2Phrase1()
{
    uint16_t k = random(1000,2000);
    
    for (uint16_t i = 0; i <=  random(100,2000); i++) {
        // tone(_pinBuzzer, k+(-i*2));          
        // delay(random(.9,2));     
        _tone(k+(-i*2),random(.9,2),0);
    } 
      
    for (uint16_t i = 0; i <= random(100,1000); i++) {
        // tone(_pinBuzzer, k + (i * 10));          
        // delay(random(.9,2)); 
        _tone(k+(i*10),random(.9,2),0);            
    } 
}

/**
 * @brief 
 * 
 */
void OttoSound::_r2d2Phrase2()
{
    uint16_t k = random(1000,2000);

    for (uint16_t i = 0; i <= random(100,2000); i++) {
        // tone(_pinBuzzer, k+(i*2));          
        // delay(random(.9,2));
        _tone(k+(i*2),random(.9,2),0);
    } 
  
    for (uint16_t i = 0; i <= random(100,1000); i++) {
        // tone(_pinBuzzer, k + (-i * 10));          
        // delay(random(.9,2)); 
        _tone(k+(-i*10),random(.9,2),0);
    } 
}


/**
 * @brief 
 * 
 */
void OttoSound::songSilentNight(void)
{
    int lead_note_count = sizeof(Silent_night_lead_notes) / sizeof(float);
    int bass_note_count = sizeof(Silent_night_bass_notes) / sizeof(float);
    int curr_lead_note = 0;
    int curr_bass_note = 0;
    uint16_t curr_lead_note_duration_remaining = Silent_night_lead_duration[curr_lead_note];
    uint16_t curr_bass_note_duration_remaining = 1500;
    float lead_freq, bass_freq;
    uint16_t duration;

    while (curr_lead_note < lead_note_count && curr_bass_note < bass_note_count) {
        lead_freq = Silent_night_lead_notes[curr_lead_note];
        bass_freq = Silent_night_bass_notes[curr_bass_note];
        duration = min(curr_lead_note_duration_remaining, curr_bass_note_duration_remaining);

        if (lead_freq > 0 && bass_freq > 0) {
            //play_two_notes(lead_freq, bass_freq, duration);
            for (unsigned long t=0; t<duration; t+=2*(POLY_DELTA+1)) {
                _tone(lead_freq, POLY_DELTA, 1);
                _tone(bass_freq, POLY_DELTA, 1);
            }
        } else if (lead_freq > 0) {
            //play_one_note(lead_freq, duration);
            _tone(lead_freq, duration, 1);
        } else if (bass_freq > 0) {
            //play_one_note(bass_freq, duration);
            _tone(bass_freq, duration, 1);
        } else {
            delay( duration );
        }
        // Advance lead note
        curr_lead_note_duration_remaining -= duration;
        if(curr_lead_note_duration_remaining < 1) {
            curr_lead_note++;
            curr_lead_note_duration_remaining = Silent_night_lead_duration[curr_lead_note];
        }
        // Advance bass note
        curr_bass_note_duration_remaining -= duration;
        if(curr_bass_note_duration_remaining < 1) {
            curr_bass_note++;
            curr_bass_note_duration_remaining = 1500;
        }
        
    } 
}

/**
 * @brief 
 * 
 */
void OttoSound::songTetris(void)
{
    int lead_note_count = sizeof(tetris_lead_notes) / sizeof(float);
    int bass_note_count = sizeof(tetris_bass_notes) / sizeof(float);
    int curr_lead_note = 0;
    int curr_bass_note = 0;
    uint16_t curr_lead_note_duration_remaining = tetris_lead_duration[curr_lead_note];
    uint16_t curr_bass_note_duration_remaining = 1500;
    float lead_freq, bass_freq;
    uint16_t duration;

    while (curr_lead_note < lead_note_count && curr_bass_note < bass_note_count) {
        lead_freq = tetris_lead_notes[curr_lead_note];
        bass_freq = tetris_bass_notes[curr_bass_note];
        duration = min(curr_lead_note_duration_remaining, curr_bass_note_duration_remaining);

        if (lead_freq > 0 && bass_freq > 0) {
            //play_two_notes(lead_freq, bass_freq, duration);
            for (unsigned long t=0; t<duration; t+=2*(POLY_DELTA+1)) {
                _tone(lead_freq, POLY_DELTA, 1);
                _tone(bass_freq, POLY_DELTA, 1);
            }
        } else if (lead_freq > 0) {
            //play_one_note(lead_freq, duration);
            _tone(lead_freq, duration, 1);
        } else if (bass_freq > 0) {
            //play_one_note(bass_freq, duration);
            _tone(bass_freq, duration, 1);
        } else {
            delay( duration );
        }
        // Advance lead note
        curr_lead_note_duration_remaining -= duration;
        if(curr_lead_note_duration_remaining < 1) {
            curr_lead_note++;
            curr_lead_note_duration_remaining = tetris_lead_duration[curr_lead_note];
        }
        // Advance bass note
        curr_bass_note_duration_remaining -= duration;
        if(curr_bass_note_duration_remaining < 1) {
            curr_bass_note++;
            curr_bass_note_duration_remaining = 250;
        }
        
    } 
}
