/**
 * @file OttoSoundSong.h
 * @author David LEVAL (dleval@senstronic.com)
 * @brief 
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

#ifndef OTTOSOUNDSONG_h
#define OTTOSOUNDSONG_h

#include <Arduino.h>
#include "OttoSoundNote.h"

// Silent Night -----------------------------------------------------------------------------

const PROGMEM float Silent_night_lead_notes[] = {
    note_G4, note_A4, note_G4, note_E4,
    note_G4, note_A4, note_G4, note_E4,
    note_D5, note_D5, note_B4,
    note_C5, note_C5, note_G4,
    note_A4, note_A4, note_C5, note_B4, note_A4, note_G4, note_A4, note_G4, note_E4,
    note_A4, note_A4, note_C5, note_B4, note_A4, note_G4, note_A4, note_G4, note_E4,
    note_D5, note_D5, note_F5, note_D5, note_B4, note_C5, note_E5,
    note_C5, note_G4, note_E4, note_G4, note_F4, note_D4, note_C4
};

const PROGMEM uint16_t Silent_night_lead_duration[] = { //120BPM
    750, 250, 500, 1500,
    750, 250, 500, 1500,
    1000, 500, 1500,
    1000, 500, 1500,
    1000, 500, 750, 250, 500, 750, 250, 500, 1500,
    1000, 500, 750, 250, 500, 750, 250, 500, 1500,
    1000, 500, 750, 250, 500, 1500, 1500,
    500, 500, 500, 750, 250, 500, 1500
};

const PROGMEM float Silent_night_bass_notes[] = {
    note_C2, note_C2, note_C2, note_C2, note_G1, note_G1, note_C2, note_C2,
    note_F1, note_F1, note_C2, note_C2, note_F1, note_F1, note_C2, note_C2,
    note_G1, note_G1, note_C2, note_C2, note_C2, note_G1, note_C2
};

const PROGMEM uint16_t Silent_night_bass_duration[] = { //120BPM
    1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500,
    1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500,
    1500, 1500, 1500, 1500, 1500, 1500, 1500
};

// Tetris -----------------------------------------------------------------------------------

const PROGMEM float tetris_lead_notes[] = {
    // part 1
    note_E5, note_B4, note_C5, note_D5, note_C5, note_B4, note_A4, note_A4, note_C5, note_E5, note_D5, note_C5, note_B4, note_B4, note_C5, note_D5, note_E5, note_C5, note_A4, note_A4, note_R,
    note_D5, note_F5, note_A5, note_G5, note_F5, note_E5, note_C5, note_E5, note_D5, note_C5, note_B4, note_B4, note_C5, note_D5, note_E5, note_C5, note_A4, note_A4, note_R,
    // part 2
    note_E4, note_C4, note_D4, note_B3, note_C4, note_A3, note_Ab3, note_B3,
    note_E4, note_C4, note_D4, note_B3, note_C4, note_E4, note_A4, note_A4, note_Ab4, note_R
};

const PROGMEM uint16_t tetris_lead_duration[] = { //120BPM
  // part 1
  500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500, 500, 500,
  750, 250, 500, 250, 250, 750, 250, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500, 500, 500,
  // part 2
  1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
  1000, 1000, 1000, 1000, 500, 500, 500, 500, 1500, 500
};

const PROGMEM float tetris_bass_notes[] = {
    // part 1
    note_E2, note_E3, note_E2, note_E3, note_E2, note_E3, note_E2, note_E3, note_A1, note_A2, note_A1, note_A2, note_A1, note_A2, note_A1, note_A2, note_Ab1, note_Ab2, note_Ab1, note_Ab2, note_Ab1, note_Ab2, note_Ab1, note_Ab2, note_A1, note_A2, note_A1, note_A2, note_A1, note_B2, note_C3, note_E3,
    note_D2, note_D3, note_D2, note_D3, note_D2, note_D3, note_D2, note_D3, note_C2, note_C3, note_C2, note_C3, note_C2, note_C3, note_C2, note_C3, note_B1, note_B2, note_B1, note_B2, note_B1, note_B2, note_B1, note_B2, note_A1, note_A2, note_A1, note_A2, note_A1, note_A2, note_A1, note_A2,
    // part 2
    note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2,
    note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_A1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2, note_Ab1, note_E2
};

const PROGMEM float tetris_bass_duration[] = {
    // part 1
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    // part 2
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250
};

#endif //OTTOSOUNDSONG_h
