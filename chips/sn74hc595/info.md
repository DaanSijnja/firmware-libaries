# Info : sn74hc595
> [sn74hc595 chip Datasheet](https://www.ti.com/product/SN74HC164)

This readme gives information about **sn74hc595** the 8-bit shift registers with 3-state output registers.

1. [Description](#discription)
1. [Files](#files)
1. [Code snippets / usage](#code-snippets--usage)
1. [Missing Features](#missing-features)
1. [Improvements](#improvements)

## Description
The sn74hc595 8-bit shift registers with 3-state output registers chip is commonly used for multiple character 7-segment displays or other applications that need a lot of output pins. These chips can also be chained in serie so you get more than 8 outputs. It has a Serial Interface to push data into the register, when the data is in place the latch pin is set so the register data is set to the outputs of the chip. In theory you can have 'unlimited' output pins with only using 3 GPIO pins of the micro-controller.

This firmware representation adds some more functionality such as:
- Having multiple sn74hc595 chained in series (total chips can be given as a template parameter)
- Sending a ```uint8_t``` for signle chips and ```uint8_t *``` for multiple chips
- Specifying how many bits must be send
- Specifying if the bits are send most significant bit first (MSB) or least significant bit first (LSB)


## Files
### [sn74hc595.hpp](sn74hc595.hpp)
This file describes al the logic of the sn74hc595 8-bit shift registers with 3-state output registers chips logica in the sn74hc565 class.

## Code snippets / usage
### Arduino : example
The code below is a super basic arduino example that uses the sn74hc565 to control some LEDs to count up in binary
```c++
#include <Arduino.h>
#include <gpio.hpp>                           //gpio wrapper
#include <sn74hc595.hpp>                      //sn74hc595 lib


uint8_t count_value = 0;

arduino::gpio ser_pin(1);           // define gpio pin for the serial out pin
arduino::gpio clk_pin(2);           // define gpio pin for the clock pin
arduino::gpio latch_pin(3);         // define gpio pin for the latch pin

//only one chip <N,GPIO> where N = 1
sn74hc595<1, arduino::gpio *> chip(&ser_pin, &clk_pin, &latch_pin);

void setup() {
    chip.init();
}


void loop() {

    chip.set_output(count_value,LSBFIRST);

    count_value++;

    delay(1000);
}
```

## Missing features
None

## Improvements

> ### send_bits function
> With the current ```set_output``` function the data send is directy latched to the outputs of the chip, this can be unwanted behaviour when sending to multiple chips\
> **Why?:** More controll for the user?