#pragma once

#include <Arduino.h>

namespace arduino
{
   class gpio_handle
    {
        public:
            /// @brief sets the gpio pin direction 
            /// @param mode can be: OUTPUT | INPUT | INPUT_PULLUP
            inline auto set_dir(const int mode) -> void {
                pinMode( pin, mode);
            }

            /// @brief set the output of the gpio pin
            /// @param value can be: LOW | HIGH
            inline auto put(const int value) -> void {
                digitalWrite(pin,value);
            }

            /// @brief returns the current logic value of the gpio pin
            /// @return bool value
            [[nodiscard]] inline auto get() noexcept -> bool {
                return digitalRead(pin);
            }

            /// @brief returns the pin number of the gpio pin
            /// @return uint8_t value
            [[nodiscard]] inline auto get_pin() noexcept -> uint8_t {
                return pin;
            }

        protected:
            uint8_t pin;

            inline constexpr gpio_handle(
                    const uint8_t pin
                ) noexcept : 
                    pin(pin)
                {}


    }; 
}

