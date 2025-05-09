# Info : hx711 chip firmware representation
> [HX711 chip Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)

This readme gives info over the HX711 loadcell amplifier 24-bit adc chip

1. [Description](#description)
1. [Files](#files)
1. [Code snippets / usage](#code-snippets--usage)
1. [Missing features](#missing-features)
1. [Improvements](#improvements)


## Description
The HX711 is an 24-Bit Analog-to-Digital Converter most commonly used in weight scale applications. It has two different input channels A and B. Low noise PGA filters. 32, 64 and 128 gain factors (although channel A can be set to 32 and 128, channel B only to 64). It uses a simple Serial Interface to communicate with. 

This firmware representation adds some more functionality such as:
- taring the chip to get a starting value
- setting a scale to convert the raw data to units such as kg, g, etc. (this needs to be calibrated).
- reading the chip multiple times to create an average reading.

## Files
### [hx711.hpp](hx711.hpp)
This file contains the firmware represtentation class for the HX711 24-Bit Analog-to-Digital Converter chip.

## Code snippets / Usage

### Important 1/2: Interrupt protection
Because the HX711 chip works with a Serial Interface it is not protected against interrupt routines and freertos shedualing related interrupts. This can cause the recieved data to be corrupt when it an interrupt happends during a reading.

I did not want to go through the hassle of working with a ton of defines to fix this. Because in my opinion it makes the code less readable. So in the [hx711.hpp](hx711.hpp) are two places with the ```//IMPORTANT:```  mark, as shown below
```c++
PREVIEW CODE MAY BE DIFFERENT FROM ACTUAL CODE
line 141 -> 158
 //IMPORTANT: add a interrupt disable here because this wil ruin the data transfer (freertos: task enter critrial section)

//pulse the clock 24 times to get the data out
data[2] = shift_in();
data[1] = shift_in();
data[0] = shift_in();

//set the next gain / channel
for( uint8_t i = 0; i < gain; i++) {
    clk->put(1);
    sleep_us(1); //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
    clk->put(0);
    sleep_us(1); //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
}

//IMPORTANT: add a interrupt enable here to reenable the interupts (freertos: task exit critrial section)
```
Change the ``` //IMPORTANT:``` marks to interrupt disable / enables or the freertos equivalent. 

### Important 2/2: Fast micro-controllers and delays
An other important issue with the Serial Interface the HX711 uses, is that faster micro-controllers will toggle the GPIO pins too fast for the HX711 chip to read properly. For most Arduino's boards (which use atmega328p chips) run at around 16Mhz, the libary without delays works fine for them.

For higher running boards such as pico, teensy or esp32 a delay of 1 us needs to be implemented at some locations in the code. These will always have this signature and the ```//REPLACE:``` mark:
```c++
sleep_us(1); //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove

OR;

sleep_ms(x); //REPLACE: with native sleep_ms function
```



### Arduino : example
The code below is a super basic arduino example that uses the hx711 libary to read a value of a loadcell and then prints this to the Serial Monitor.
```c++
#include <Arduino.h>
#include <gpio.h>                       //gpio wrapper
#include <hx711.h>                      //hx711 lib

inline constexpr float SCALE_FACTOR = 1.0; // calulated with weight calibration


arduino::gpio data_out(1);       // define gpio pin for the data out pin
arduino::gpio clk(2);           // define gpio pin for the clock pin

hx711<arduino::gpio *> loadcell(&data_out, &clk);

void setup() {
    Serial.begin(9600);
    loadcell.init();
    loadcell.tare();
    loadcell.set_scale(SCALE_FACTOR);
}


void loop() {
    const float loadcell_value = loadcell.get_units(10);
    Serial.print("Loadcell value: ");
    Serial.print(loadcell_value);
    Serial.println(" units");   // change units to kg, g etc.
    delay(500);
}
```


## Missing features
Some missing features are:

> ### Sample rate GPIO pin
> The HX711 can run at 10sps or 80sps this is determined by the RATE pin on the chips logic voltage value. This could be set with a GPIO pin interface\
> **Important?:** Not really most of the times its connectend to GND or VCC of the PCB board thus not changeable

> ### Setting Channel and Gain
> The HX711 chip has the very limited ability to change the channel and set a gain. Channel A can be set to a gain of 32 or 128 and channel B can be set to a gain of 64. The basic functionality is in place but there is currently no way for a user to set this.\
> **Important?:** Yes, this is a main component of the HX711 chips functionality but it goes in most applications unused because only one loadcell is used.

> ### wait_ready() timeout
> The current implementation of the ```wait_ready()``` function is a blocking while loop, this can cause for unwanted behaviour by blocking the entire code flow. This can be fixed by implementing a timeoout into this function\
> **Important?:** Sort of

## Improvements
See [Missing features](#missing-features)

> ### Delays
> Currently there are some delays in the code that are needed for fast micro-controllers. These need to be manualy set by the user in the code for the libary to work which can be a hassle. These can be moved to one function to be only changed in one place.\
> **Why?:** Makes it easier for the user to change the libary for different platforms.

> ### Interrupt protection
> Same with [Delays](#delays) but now for interrupt protection. This could be moved to a section that is more clear to the user what and where to change. \
> **Why?:** Makes it easier for the user to change the libary for different platforms.
