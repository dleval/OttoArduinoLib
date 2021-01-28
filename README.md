<a href ="http://dle-dev.com">
  <img src="https://dle-dev.com/wp-content/uploads/2020/05/dle_dev_logo_v1.1.png" alt="DLE-Dev logo" title="DLE-Dev" align="right" height="40" />
</a>

Otto Robot Arduino Library
======================
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![version](https://img.shields.io/badge/version-1.0-blue)

:star: Star us on GitHub — it helps!

Based on the original Otto the robot project library : <a href="https://github.com/OttoDIY/OttoDIYLib">OttoDIYLib</a>

<a href="https://dle-dev.com>
PHOTO !!!
</a>
  
## Table of content
- [Version](#version)
- [Hardware](#hardware)
    - [Compatibility](#compatibility)
    - [Wiring](#wiring)
- [Installation](#installation)
- [How to Contribute](#HowtoContribute)
- [Dependencies](#dependencies)
- [License](#license)
- [Links](#links)


## Version

- Version 1.0 -- Intial version
    
## Hardware

Arduino NANO (or UNO)

### Compatibility

Robot         | Include use
------------- | -------------
Otto          | "Otto.h"
Otto Lee      | "OttoLee.h"

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
Head for Otto Lee |  8 

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

## Dependencies

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

* [Web site](https://dle-dev.com/index.php/2020/12/01/esp-clock/)

