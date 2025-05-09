#pragma once

#include "gpio_handle.hpp"

namespace pico
{
	class gpio :
		public gpio_handle
	{
	public:
		explicit gpio(const uint pin) noexcept :
			gpio_handle(pin)
		{
			gpio_init(pin);
		}


		~gpio() noexcept
		{
			deinit();
		}


		// No copy
		gpio(const gpio&) = delete;
		auto operator=(const gpio&) = delete;


		// Move allowed
		gpio(gpio&& other) noexcept :
			gpio_handle(other.pin)
		{
			other.pin = -1;
		}


		auto operator=(gpio&& other) noexcept -> gpio&
		{
			deinit();

			pin = other.pin;
			other.pin = -1;

			return *this;
		}


	private:
		auto deinit() noexcept -> void
		{
			if (pin != -1)
			{
				gpio_deinit(pin);
			}
		}
	};
}
