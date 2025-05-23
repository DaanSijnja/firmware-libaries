#pragma once

#include <Arduino.h>

namespace arduino
{
   class adc_handle
    {
        public:
            /// @brief Sets the adc mode
            /// @param mode int | can be: INPUT | INPUT_PULLUP
            inline auto init(const int mode) -> void {
                if(mode == INPUT || mode == INPUT_PULLUP){
                    pinMode( pin, mode);
                } else { pinMode( pin, INPUT); }
            }

            /// @brief Returns the analog value of the pin
            /// @return unsigned int | 
            [[nodiscard]] inline auto get() noexcept -> unsigned int {
                return analogRead(pin);
            }  

            /// @brief returns the pin number of the adc pin
            /// @return uint8_t |
            [[nodiscard]] inline auto get_pin() noexcept -> uint8_t {
                return pin;
            }

        protected:
            uint8_t pin;

            inline constexpr adc_handle(
                    const uint8_t pin
                ) noexcept : 
                    pin(pin)
                {}
    }; 
}