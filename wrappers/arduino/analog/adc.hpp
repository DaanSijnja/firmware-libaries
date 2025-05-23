#pragma once

#include "adc_handle.hpp"

namespace arduino
{
    class adc :
            public adc_handle
    {   
        public: 
            explicit adc(const uint8_t pin) noexcept :
                adc_handle(pin)
            { }

            adc(const adc&) = delete;
            adc& operator=(const adc&) = delete; 

            /// @brief Sets the refrence voltage of the build in ADC
            /// @param mode int | can be DEFAULT or EXTERNAL
            static auto set_reference(const int mode) -> void {
                analogReference(mode);
            }

            adc(adc&& other) noexcept :
                adc_handle(other.pin)
            {
                other.pin = -1;
            }
    };
}