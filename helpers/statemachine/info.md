# Info : AVR Statemachine 
This readme gives information about **AVR Statemachine**

1. [Discription](#discription)
1. [Files](#files)
1. [Code snippets / usage](#code-snippets--usage)

## Discription
These helper classes can create a so called statemachine which can be used to manage different states the firmware can be in. This is writen for AVR / single core micro processors and not for micro processors using FreeRTOS. In FreeRTOS you would use an other system that uses the FreeRTOS playing rules

## Files
### [base_state.hpp](base_state.hpp)
Contains the virual `base_state` class used for creating statemachine states

### [base_statemachine.hpp](base_statemachine.hpp) 
Contains the virual `base_statemachine` class used for creating a statemachine

## Code snippets / usage
Some code snippets and usage info

### Empty Statemachine

These `.hpp` and `.cpp` files show the basic structure of an empty statemachine

#### .hpp file
```c++

#pragma once

#include <base_statemachine.hpp>

class empty_statemachine
   : public statemachine::base_statemachine {

  public:
    empty_statemachine() noexcept :
        //=> your variable constructors here
    {}

    auto initialize() noexcept -> void;
    auto update() noexcept -> void;
    auto change_state(statemachine::base_state& new_state ) -> void;

    //=> Add the functions that states can use here


  private:
    //=> Add your variables that you want to use

};

```

#### .cpp file
```c++
#include "empty_statemachine.hpp"
#include "empty_state.hpp"


/// @brief Initalizes the station
auto empty_statemachine::initialize() noexcept -> void {
    //=> Initialize the components

    //=> set the current state to the starting state
    current_state = &statemachine::empty_state::get_instance();
    current_state->enter((void *)this);
}

/// @brief Updates the station
auto empty_statemachine::update() -> void {
    //=> update any global things used in states
    
    //=> update the current state
    state_update( (void *) this );
}

/// @brief Changes the current state of the station
/// @param new_state the new state the station need to be changed to
auto empty_statemachine::change_state(statemachine::base_state& new_state ) -> void {

    //=> sets the new state
    set_state(new_state, (void*) this);
}


//=> add function definitions here

```

### Empty State class
These `.hpp` and `.cpp` files show the basic structure of an empty state

#### .hpp file
```c++
#pragma once
#include <base_state.hpp>
#include "empty_statemachine.hpp"   //=> include your statemachine header

namespace statemachine {

    class empty_state 
    : public base_state {

        public:
            auto enter(void* stm) -> void;
            auto update(void* stm) -> void;
            auto exit(void* stm) -> void;

            static base_state& get_instance() {
                static empty_state singleton;
                return singleton;
            }

        private:
            //=> add your own variables used in the state here - all private because other states should not have access to these variables. If multiple states need access to one variable place this in the state machine

            empty_state() {};
            empty_state(const empty_state & other);
            empty_state & operator=(const empty_state& other);
    };

}
```
#### .cpp file
```c++
#include "empty_state.hpp" //=> 

namespace statemachine {

    auto empty_state::enter(void* stm) -> void {
        auto _statemachine = (empty_statemachine*) stm; //=> Get your statemachine pointer from the parameter. So you can use it in this function

        //=> This function is called once when the state is entered
    }   

    auto empty_state::update(void* stm) -> void {
        auto _statemachine = (empty_statemachine*) stm; //=> Get your statemachine pointer from the parameter. So you can use it in this function

        //=> This function is called every update call form the statemachine
    }

    auto empty_state::exit(void* stm) -> void {
        auto _statemachine = (empty_statemachine*) stm; //=> Get your statemachine pointer from the parameter. So you can use it in this function

        //=> This function is called once when the state is exited
    }

}
```

### Arduino IDE code example

This example shows how to turn a LED on or off using a button and states (normaly this problem is too simple for a statemachine solution so it purely an example)

#### General state header file `states.hpp`
This header file houses all the states declarations
```c++
    #pragma once
#include <base_state.hpp>
#include "lamp_statemachine.hpp"   //=> include your statemachine header

namespace statemachine {

    class led_off_state 
    : public base_state {

        public:
            auto enter(void* stm) -> void;
            auto update(void* stm) -> void;
            auto exit(void* stm) -> void;

            static base_state& get_instance() {
                static led_off_state singleton;
                return singleton;
            }

        private:
            //=> add your own variables used in the state here - all private because other states should not have access to these variables. If multiple states need access to one variable place this in the state machine

            led_off_state () {};
            led_off_state (const led_off_state  & other);
            led_off_state  & operator=(const led_off_state & other);
    };

    class led_on_state 
    : public base_state {

        public:
            auto enter(void* stm) -> void;
            auto update(void* stm) -> void;
            auto exit(void* stm) -> void;

            static base_state& get_instance() {
                static led_on_state singleton;
                return singleton;
            }

        private:
            //=> add your own variables used in the state here - all private because other states should not have access to these variables. If multiple states need access to one variable place this in the state machine

            led_on_state () {};
            led_on_state (const led_on_state  & other);
            led_on_state  & operator=(const led_on_state & other);
    };

}
    
```

#### led off state cpp `led_off_state.hpp`
This file has the implementation of the `led_off_state` state class
```c++
#include "states.hpp" //=> 

namespace statemachine {

    auto led_off_state::enter(void* stm) -> void {
        auto _statemachine = (lamp_statemachine*) stm; 
        _statemachine->set_led(false);

    }   

    auto led_off_state::update(void* stm) -> void {
        auto _statemachine = (lamp_statemachine*) stm;

        if(_statemachine->get_button()) {
            _statemachine->change_state(led_on_state::get_instance());
            return;         //=> return after state switch to prevent wierd behaviour
        }
    }

    auto led_off_state::exit(void* stm) -> void {
        [[maybe_unused]] auto _statemachine = (lamp_statemachine*) stm; 
    }

}
```

#### led on state cpp `led_on_state.hpp`
This file has the implementation of the `led_on_state` state class
```c++
#include "states.hpp" //=> 

namespace statemachine {

    auto led_on_state::enter(void* stm) -> void {
        auto _statemachine = (lamp_statemachine*) stm;
        _statemachine->set_led(true);
    }   

    auto led_on_state::update(void* stm) -> void {
        auto _statemachine = (lamp_statemachine*) stm; 

        if(_statemachine->get_button()) {
            _statemachine->change_state(led_off_state::get_instance());
            return;         //=> return after state switch to prevent wierd behaviour
        }
    }

    auto led_on_state::exit(void* stm) -> void {
        [[maybe_unused]] auto _statemachine = (lamp_statemachine*) stm; 
    }

}
```

#### Lamp Statemachine header file `lamp_statemachine.hpp`
This header file has the `lamp_statemachine` declarations
```c++
#pragma once

#include <base_statemachine.hpp>

class lamp_statemachine
   : public statemachine::base_statemachine {

  public:
    lamp_statemachine() noexcept :
        LED_PIN(0),
        BUTTON_PIN(1)
    {}

    auto initialize() noexcept -> void;
    auto update() noexcept -> void;
    auto change_state(statemachine::base_state& new_state ) -> void;

    //=> Add the functions that states can use here
    auto set_led(bool value) -> void;
    auto get_button() -> bool;

  private:
    const int LED_PIN;
    const int BUTTON_PIN;
    //=> Add your variables that you want to use

};
```

#### Lamp Statemachine cpp file `lamp_statemachine.cpp`
This file has the `lamp_statemachine` function implementations
```c++
#include "lamp_statemachine.hpp"
#include "states.hpp"


/// @brief Initalizes the station
auto lamp_statemachine::initialize() noexcept -> void {

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    current_state = &statemachine::led_off_state::get_instance();
    current_state->enter((void *)this);
}

/// @brief Updates the station
auto lamp_statemachine::update() -> void {
    
    state_update( (void *) this );
}

/// @brief Changes the current state of the station
/// @param new_state the new state the station need to be changed to
auto lamp_statemachine::change_state(statemachine::base_state& new_state ) -> void {
    set_state(new_state, (void*) this);
}

auto lamp_statemachine::set_led(const bool value) -> void {
    digitalWrite(LED_PIN, value);
}

auto lamp_statemachine::get_button() -> bool {
    return digitalRead(BUTTON_PIN);
}

```

#### Arduino main code ``main.cpp`
```c++
#include <Arduino.h>
#include "lamp_statemachine.hpp"

lamp_statemachine lamp;

void setup() {
    lamp.initialize();
}

void loop() {
    lamp.update();
}

```



