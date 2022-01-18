/**
 * @file i2c_slave.ino
 * @author David LEVAL (dleval@dle-dev.com)
 * @brief Otto Calibration Tool
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
#include <OttoLeeLib.h>
#include <Wire.h>

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

// I2C configuration ---------------------------------------------------
#define I2C_ADDRESS 8

// Otto driver object --------------------------------------------------
OttoLee otto(PIN_LEG_L, PIN_LEG_R, PIN_FOOT_L, PIN_FOOT_R, PIN_ARM_L, PIN_ARM_R, PIN_HEAD, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo);

// I2C Function prototypes ---------------------------------------------
void i2cReceiveEvent(int numBytes);
void i2cRequestEvent(void);


void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(i2cReceiveEvent);
  Wire.onRequest(i2cRequestEvent);

  otto.init(true);
  otto.home();

}

void loop() {
  // put your main code here, to run repeatedly:

}

// I2C Function  -------------------------------------------------------

void i2cReceiveEvent(int numBytes)
{
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void i2cRequestEvent(void)
{
  Wire.write("hello "); // respond with message of 6 bytes
}
