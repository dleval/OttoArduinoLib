<a href ="http://dle-dev.com">
  <img src="https://dle-dev.com/wp-content/uploads/2020/05/dle_dev_logo_v1.1.png" alt="DLE-Dev logo" title="DLE-Dev" align="right" height="40" />
</a>

Otto Robot Arduino Library
======================
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![version](https://img.shields.io/badge/version-1.1-blue)

:star: Star us on GitHub — it helps!

Based on the original Otto the robot project library : [Otto DIY](https://github.com/OttoDIY/OttoDIYLib)

> :warning: **This is not the original library.** <br>
> Implementation of the backward compatibility with the OttoDIYLib library : ![20%](https://progress-bar.dev/20)

[![Otto Robot](https://dle-dev.com/wp-content/uploads/2021/05/otto001.png)](https://dle-dev.com/)
  
## Table of content
- [Otto Robot Arduino Library](#otto-robot-arduino-library)
	- [Table of content](#table-of-content)
	- [Version](#version)
	- [Hardware](#hardware)
		- [Compatibility](#compatibility)
		- [Wiring](#wiring)
	- [Installation](#installation)
	- [Usage](#usage)
		- [Adding library](#adding-library)
		- [Configuration and object](#configuration-and-object)
		- [Initialization](#initialization)
		- [Predetermined Motion Functions](#predetermined-motion-functions)
		- [Sound](#sound)
		- [Distance Sensor](#distance-sensor)
	- [How to Contribute](#how-to-contribute)
	- [License](#license)
	- [Links](#links)


## Version

- Version 1.0 -- Intial version
    
## Hardware

Arduino NANO (or UNO)

### Compatibility

Robot         | Include use
------------- | -------------
Otto          | "OttoLib.h"
Otto Lee      | "OttoLeeLib.h"

### Wiring

It is possible to make a custom wiring. The examples provided use the following wiring:

Servos | Pin
------------- | -------------
Left Leg or Wheel |  2 
Right  Leg or Wheel |  3
Left Foot | 4 
Right Foot |  5 
Left arm for Otto Lee  |  6 
Right arm for Otto Lee |  7 
Head for Otto Lee |  10 

Ultrasonic | Pin
------------- | -------------
Trigger | 8 
Echo | 9  

Buzzer | Pin
------------- | -------------
Positive | 13 

## Installation

Arduino IDE :
1. [Download the .zip here](https://github.com/DLE-Dev/OttoArduinoLib/archive/main.zip)
2. Open Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library. At the top of the drop down list, select the option to "Add .ZIP Library''.
3. You will be prompted to select the library. Navigate to the .zip file's location, that you just downloaded and open it.
4. Return to the Sketch > Include Library menu. menu. You should now see the library at the bottom of the drop-down menu. That means is ready to use example codes!
[for more details or other ways visit this link](https://www.arduino.cc/en/Guide/Libraries) 

Platform IO :
1. Edit the "platformio.ini" file of your project. 
2. Add parameters: 
```
lib_deps =
	dleval/OttoArduinoLib
```

## Usage

### Adding library

Add the header corresponding to the robot model at the start of the Arduino sketch. 

*Robot Otto :*
```
#include <OttoLib.h>
```
*Robot Otto Lee :*
```
#include <OttoLeeLib.h>
```

### Configuration and object

When creating the robot control object, all the pins must be configured.<br/>
Example :
```
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

// Otto driver object --------------------------------------------------
OttoLee otto(PIN_LEG_L, PIN_LEG_R, PIN_FOOT_L, PIN_FOOT_R, PIN_ARM_L, PIN_ARM_R, PIN_HEAD, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo);
```

### Initialization

When starting the program, the 'init' function must be called with the use of servomotor calibration as a parameter. <br/>
It is best to place the servomotors in their home position after initialization wwith 'home' function.

```
void setup() {
  otto.init(true);
  otto.home();
}
```

### Predetermined Motion Functions

Many preconfigured movements are available in the library: 
- jump with arguments(steps, period)
- walk with arguments(steps, period, direction, arm oscillation, head oscillation);
- turn with arguments(steps, period, direction, arm oscillation, head oscillation);
- bend with arguments(steps, period, direction);
- shakeLeg with arguments(steps, period, direction);
- updown with arguments(steps, period, height);
- handsup
- handwave with argument(direction);
- swing with arguments(steps, period, height);
- tiptoeSwing with arguments(steps, period, height);
- jitter with arguments(steps, period, height);
- ascendingTurn with arguments(steps, period, height);
- moonwalker with arguments(steps, period, height, direction);
- crusaito with arguments(steps, period, height, direction);
- flapping with arguments(steps, period, height, direction);
- headNo with arguments(steps, period);

Example for movements of 10 jumps:
```
otto.jump(10,2000);
```

### Sound

Otto can emit several sounds with the 'sing' function.
```
otto.sing(nameOfSound);
```
List of pre-recorded sounds:
- S_connection
- S_disconnection
- S_buttonPushed
- S_mode1 
- S_mode2
- S_mode3
- S_surprise
- S_OhOoh
- S_OhOoh2
- S_cuddly
- S_sleeping
- S_happy
- S_superHappy
- S_happy_short
- S_sad
- S_confused
- S_fart1
- S_fart2
- S_fart3

It is possible to emit ons like the famous R2D2 robot with the 'r2d2' function.
```
otto.r2d2();
```

The library also includes two songs: Silent Night and Tetris.
```
otto.songSilentNight();
otto.songTetris();
```

### Distance Sensor

The 'getDistance' function allows the use of the ultrasonic sensor. 
```
float distance;
distance = otto.getDistance();
```


## How to Contribute

Contributing to this software is warmly welcomed. There are 3 ways you can contribute to this project:
1. Test and if find a problem then post an issue.
2. Helps us solve the issues or other bugs by commenting
3. Bring missing libraries from other Otto remixes here.
You can do this [basically by forking](https://help.github.com/en/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/en/articles/about-pull-requests). Please add a change log and your contact into file header.

Thanks for your contribution.

## License

The OttoArduinoLib Library is licensed under the terms of the GPL Open Source
license and is available for free.

## Links

* [Otto DIY](https://www.ottodiy.com/)
* [Otto DIY Github](https://github.com/OttoDIY/OttoDIYLib)
* [DLE-Dev](https://dle-dev.com/)

