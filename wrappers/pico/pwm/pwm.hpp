#pragma once

#include "pwm_handle.hpp"

namespace pico
{
    class pwm :
		public pwm_handle
	{
	public:
		explicit pwm(uint8_t pin) noexcept :
			pwm_handle(pin)
		{
		}



		~pwm() noexcept
		{
			deinit();
		}


		// No copy
		pwm(const pwm&) = delete;
		auto operator=(const pwm&) = delete;


		// Move allowed
		pwm(pwm&& other) noexcept :
            pwm_handle(other.pin)
		{
			other.pin = 0;
		}


		auto operator=(pwm&& other) noexcept -> pwm&
		{
			deinit();

			pin = other.pin;
            slice = pwm_gpio_to_slice_num(pin);
            channel = pwm_gpio_to_channel(pin);
			other.pin = 0;
            other.slice = 0;
            other.channel = 0;
            
			return *this;
		}


	private:
		auto deinit() noexcept -> void
		{

		}
	};





}