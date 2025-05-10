# Info : arduino gpio wrapper
This readme gives information about **arduino gpio wrapper**

1. [Discription](#discription)
1. [Files](#files)
1. [code snippets / usage](#code-snippets--usage)
1. [Missing Features](#missing-features)
1. [Improvements](#improvements)



## Discription
This is a wrapper for the Arduino IDE gpio functions to make them better readablity, more convenient in use and better maintainable

## Files
### [gpio.hpp](gpio.hpp)
This file has the gpio class thats used for creating a gpio_handle which can be used for gpio related functions. This class also stops the handle from being copied or moved because this can cause complications.

### [gpio_handle.hpp](gpio_handle.hpp) 
This file has the gpio_handle class that is the core wrapper for the arduino gpio functionality. It makes gpio usage more easy to use and gives better readablity when writing firmware. Does not support PWM or Analoge reading at the moment thinking of implementing this here or in other wrappers to have more consistency with other API/IDE wrappers I wrote

## code snippets / usage
Some code snippets and usage info
### New Syntax
```c++
// New Syntax                           What it would look like in default Arduino IDE

// initalization --                     
arduino::gpio pin(0);                   // #define PIN 0

//setting mode / direction --
pin.set_dir(OUTPUT);                    // pinMode( PIN, OUTPUT );

//set output of pin --
pin.put(HIGH);                          // digitalWrite( PIN, HIGH );

//reading pin value (digital) --
bool value = pin.get();                 // bool value = digitalRead( PIN )
```
### blink led snippet
```c++
#include <Arduino.h>
#include <gpio.hpp>         // include the gpio wrapper


arduino::gpio led(0);       // define gpio pim

void setup() {
    led.set_dir(OUTPUT);    // set led to output
}


void loop() {
    led.put(HIGH);          // set the output of the gpio pin to logic high
    delay(500);
    led.put(LOW);           // set the output of the gpio pin to logic low
    delay(500);
}
```


## Missing features
todo 


## Improvements
todo



