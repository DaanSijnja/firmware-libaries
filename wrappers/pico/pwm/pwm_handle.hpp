#pragma once

#include "hardware/pwm.h"
#include "pico/stdlib.h"

//TODO
// update this with the functionality to set PWM with configs


namespace pico {
    class pwm_handle {
        public:

        auto init_gpio() -> void {
            gpio_set_function(pin, static_cast<gpio_function_t>(gpio_function::PWM) );
        }

        auto set_wrap(uint16_t wrap) noexcept -> void {
            pwm_set_wrap(slice, wrap);
        }

        // auto set_frequency() -> void;
        
        auto set_level(uint16_t level) noexcept -> void {
            pwm_set_chan_level(slice, channel, level);
        }

        auto set_clock_divider(float clk_div) noexcept -> void {
            pwm_set_clkdiv(slice, clk_div);
        }

        auto set_phase_correct(bool phase_correct) noexcept -> void {
            pwm_set_phase_correct(slice, phase_correct);
        }

        auto set_enabled(bool enabled) noexcept -> void {
            pwm_set_enabled(slice, enabled);
        }

        protected:
            uint8_t pin;
            uint8_t slice;
            uint8_t channel;

            explicit constexpr pwm_handle(const uint pin) noexcept :
			    pin( pin ),
                slice( pwm_gpio_to_slice_num( pin ) ),
                channel( pwm_gpio_to_channel( pin ) )
		    {  
		    }
    };
}