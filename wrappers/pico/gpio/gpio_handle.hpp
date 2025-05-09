#pragma once

#include "pico/stdlib.h"
#include "pico/gpio_common.hpp"


namespace pico
{
	class gpio_handle
	{
	public:
		inline auto set_function(const gpio_function f) noexcept -> void
		{
			gpio_set_function(pin, static_cast<gpio_function_t>(f));
		}

		inline auto set_dir(const bool out) noexcept -> void
		{
			gpio_set_dir(pin, out);
		}

		inline auto put(const bool state) noexcept -> void
		{
			gpio_put(pin, state);
		}

		[[nodiscard]] inline auto get() noexcept -> bool
		{
			return gpio_get(pin);
		}

	protected:
		uint pin;

		explicit constexpr gpio_handle(const uint pin) noexcept :
			pin(pin)
		{

		}
	};
}
