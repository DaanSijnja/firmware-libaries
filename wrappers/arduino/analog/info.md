# Info : arduino adc wrapper
This readme gives information about **arduino adc wrapper**

1. [Discription](#discription)
1. [Files](#files)
1. [Code snippets / usage](#code-snippets--usage)

## Discription
This is a wrapper for the Arduino IDE internal adc functions to make them better in readablity, more convenient in use and better maintainable

## Files
### [adc.hpp](adc.hpp)
This file has the adc class thats used for creating a adc_handle which can be used for adc related functions. This class also stops the handle from being copied or moved because this can cause complications.

### [adc_handle.hpp](adc_handle.hpp) 
This file has the adc_handle class that is the core wrapper for the arduino gpio functionality. It makes internal adc usage more easy to use and gives better readablity when writing firmware. 

## Code snippets / usage
Some code snippets and usage info
### New Syntax
```c++
// New Syntax                           What it would look like in default Arduino IDE

// initalization --                     
arduino::adc analog_input(0);             // #define PIN 0

//setting mode / direction --
analog_input.init(INPUT);                 // pinMode( PIN, INPUT );

//reading pin value (analog) --
unsigned int value = analog_input.get();  // unsigned int value = analogRead( PIN )
```

### Read analog value snippet
```c++
#include <Arduino.h>
#include <adc.hpp>                                      // include the adc wrapper

arduino::adc analog_input(0);                           // define adc pim

void setup() {
    Serial.begin(9600);                                 // Start Serial monitor
    analog_input.init(INPUT);                           // set led to output
}


void loop() {
    const unsigned int value = analog_input.get();      // get the analog input
    Serial.print("Analog Value: ");
    Serial.println(value);
}
```


