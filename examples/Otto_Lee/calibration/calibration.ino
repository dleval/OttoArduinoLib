/**
 * @file calibration.ino
 * @author David LEVAL (dleval@dle-dev.com)
 * @brief Otto Lee Calibration Tool
 * @version 1.0
 * @date 2021-01-25
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

// Pinout configuration ------------------------------------------------
#define PIN_LEG_L         2   // Left leg servo
#define PIN_LEG_R         3   // Right leg servo
#define PIN_FOOT_L        4   // Left foot servo
#define PIN_FOOT_R        5   // Right foot servo
#define PIN_ARM_L         6   // Left arm servo
#define PIN_ARM_R         7   // Right arm servo
#define PIN_HEAD          10  // Head servo
#define PIN_Trigger       8   // Ultrasound distance sensor (Trigger)
#define PIN_Echo          9   // Ultrasound distance sensor (Echo)
#define PIN_NoiseSensor   A6
#define PIN_Buzzer        13  // Buzzer

// Global variables ----------------------------------------------------
uint8_t positions[_NBR_OF_SERVO] = {90, 90, 90, 90, 90, 90, 90};
int8_t trims[_NBR_OF_SERVO] = {0, 0, 0, 0, 0, 0, 0};
uint8_t servoSelection;

// Otto driver object --------------------------------------------------
OttoLee otto(PIN_LEG_L, PIN_LEG_R, PIN_FOOT_L, PIN_FOOT_R, PIN_ARM_L, PIN_ARM_R, PIN_HEAD, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo);

// Function prototypes -------------------------------------------------
void helpMenu(void);
void displayTrimsValues(void);
void processUserCalibration(char c);
void displayServoSelection(uint8_t selection);
void displayServoTrimSelected(uint8_t selection);
void setTrimsCalibration(void);

// Setup Function ------------------------------------------------------
void setup() {
  Serial.begin(115200);

  otto.init(true);
  otto.home();

  helpMenu();

  //Read all Trims values from EEPROM
  for(uint8_t i=0; i<_NBR_OF_SERVO; i++) trims[i] = otto.loadTrimsFromEEPROM(i);
  //Then apply Trims
  setTrimsCalibration();
  //Display Trims
  displayTrimsValues();
}

// Main loop program ---------------------------------------------------
void loop() {
  if(Serial.available() > 0) {
    processUserCalibration(Serial.read());
  }
}

/**
 * @brief 
 * 
 */
void helpMenu(void)
{
  Serial.println(F("This is used to set the trim on the servos for 'Otto Lee'"));
  Serial.println(F("The interface uses single character controls"));
  Serial.println("");
  Serial.println(F("'e' - set all trims to 0"));
  Serial.println(F("'r' - read EEPROM trims and use them"));
  Serial.println(F("'h' - this message"));
  Serial.println(F("'+' - add to the trim"));
  Serial.println(F("'-' - subtract from the trim"));
  Serial.println(F("'0' - trim the Left Leg"));
  Serial.println(F("'1' - trim the Right Leg"));
  Serial.println(F("'2' - trim the left Foot"));
  Serial.println(F("'3' - trim the Right Foot"));
  Serial.println(F("'4' - trim the Left Arm"));
  Serial.println(F("'5' - trim the Right Arm"));
  Serial.println(F("'6' - trim the Head"));
  Serial.println(F("'w' - write the current trims to  EEPROM"));
  Serial.println("");
}

/**
 * @brief 
 * 
 */
void displayTrimsValues(void)
{
  Serial.print(F("Trim values : "));
  for(uint8_t i=0; i<_NBR_OF_SERVO; i++) {
    Serial.print(trims[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

/**
 * @brief 
 * 
 * @param c 
 */
void processUserCalibration(char c)
{
  switch(c) {
    case 'h': 
      helpMenu(); 
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5': 
    case '6': 
      Serial.println("");
      displayServoTrimSelected(c & 0x0F);
      break;
    case '+':
      trims[servoSelection]++;
      setTrimsCalibration();
      displayServoTrimSelected(servoSelection);
      break;
    case '-':
      trims[servoSelection]--;
      setTrimsCalibration();
      displayServoTrimSelected(servoSelection);
      break;
    case 'r':
    case 'R':
      Serial.println("");
      for(uint8_t i=0; i<_NBR_OF_SERVO; i++) trims[i] = otto.loadTrimsFromEEPROM(i);
      Serial.print(F("Load from EEPROM "));
      setTrimsCalibration();
      displayTrimsValues();
      break;
    case 'w':
    case 'W':
      otto.saveTrimsOnEEPROM();
      Serial.println("");
      Serial.print(F("Save to EEPROM "));
      displayTrimsValues();
      break;
    case 'e':
    case 'E':
    Serial.println("");
      for(uint8_t i=0; i<_NBR_OF_SERVO; i++) trims[i] = 0;
      Serial.print(F("Reset "));
      setTrimsCalibration();
      displayTrimsValues();
      break;
    case '\n':
    case '\r':
      break;
    default:
      Serial.print("Unknown command (");
      Serial.print(c);
      Serial.println(")");
      helpMenu();
      break;
  }
}

/**
 * @brief 
 * 
 * @param selection 
 */
void displayServoTrimSelected(uint8_t selection)
{
  servoSelection = selection;

  Serial.print("\r");

  if(servoSelection == 0) Serial.print(F("Selected Left Leg"));
  if(servoSelection == 1) Serial.print(F("Selected Right Leg"));
  if(servoSelection == 2) Serial.print(F("Selected Left Foot"));
  if(servoSelection == 3) Serial.print(F("Selected Left Foot"));
  if(servoSelection == 4) Serial.print(F("Selected Left Arm"));
  if(servoSelection == 5) Serial.print(F("Selected Right Arm"));
  if(servoSelection == 6) Serial.print(F("Selected Head"));

  Serial.print(F(" -> Trims = "));
  Serial.print(trims[servoSelection]);
  Serial.print(F("            "));
}

/**
 * @brief Set the Trims Calibration object
 * 
 */
void setTrimsCalibration(void)
{
  otto.setTrims(trims);
  otto.moveServos(200, positions);
}
