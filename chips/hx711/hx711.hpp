#pragma once

/// @brief The firmware representation class for the HX711 chip
/// @tparam GPIO gpio wrapper class
template<class GPIO>
class hx711
{
    public:

        /// @param dout_pin GPIO pin representation for the dout_pin. aka data out pin
        /// @param clk_pin GPIO pin representation for clk_pin. aka the clock pin
        hx711(const GPIO dout_pin, const GPIO clk_pin) noexcept :
        dout(dout_pin),
        clk(clk_pin),
        gain(1),
        offset(0),
        scale(1.0f)
        {}

        /// @brief Initalizes the GPIO pins 
        auto init() noexcept -> void {
            dout->set_dir(false);
            clk->set_dir(true);
        }

        /// @brief Check if the HX711 chip has new data
        /// @return bool | true if new data is available
        [[nodiscard]] auto is_ready() -> bool{
            return dout->get() == 0;
        }   
        
        /// @brief Waits till the chip is ready (blocking while)
        /// @param delay_ms ulong | the delay in ms between every check
        auto wait_ready(const unsigned long delay_ms = 0) noexcept -> void {
            while (!is_ready()) {
                sleep_ms(delay_ms);                                                     //REPLACE: with native sleep_ms function
            }
        }

        /// @brief Reads the HX711 value multiple times and then takes the average
        /// @param total_reads uint8_t | <default = 10> total times samples it takes to average
        /// @return long | value of the raw HX711 reading
        auto read_average(const uint8_t total_reads = 10) noexcept -> long {
            long sum = 0;

            for(uint8_t i = 0; i < total_reads; i++) {
                sum += read();
            }
            return sum / total_reads;
        }

        /// @brief Tares the HX711 
        /// @param total_reads uint8_t | <default = 10> total times samples it to tare the HX711
        auto tare(const uint8_t total_reads = 10) noexcept -> void {
                long sum = read_average(total_reads);
                set_offset(sum);
        }   

        /// @brief Reads the loadcell value from the HX711
        /// @param total_reads uint8_t | <default = 1> total times samples it to read the HX711
        /// @return long | value of the HX711 with the tare offset included
        [[nodiscard]] auto get_value(const uint8_t total_reads = 1) noexcept -> long {
            return read_average(total_reads) - offset;
        }

        /// @brief Reads the loadcell value in units from the HX711
        /// @param total_reads uint8_t | <default = 1> total times samples it to read the HX711
        /// @return float | value of the HX711 with the tare offset included and scaled to units
        [[nodiscard]] auto get_units(const uint8_t total_reads = 1) noexcept -> float {
            return (get_value(total_reads) / scale);
        }

        /// @brief Gets the unit scale used for converting raw HX711 Loadcell values to units
        /// @return float | scale value
        [[nodiscard]] auto get_scale() noexcept -> float {
            return scale;
        }

        /// @brief Gets the offset thats used for the zero position of the raw HX711 Loadcell values
        /// @return long | offset value
        [[nodiscard]] auto get_offset() noexcept -> long {
            return offset;
        }

        /// @brief Sets the scale factor used for converting raw HX711 loadcell values to units
        /// @param scale_factor float | scale factor
        auto set_scale(float scale_factor) noexcept -> void {
            scale = scale_factor;
        }
        
        /// @brief Sets the offset factor used for moving the zero value of the HX711 loadcell
        /// @param offset_factor long | offset factor
        auto set_offset(long offset_factor) noexcept -> void {
            offset = offset_factor;
        }

        /// @brief Powers down the HX711 chip so it consumes less power
        auto power_down() noexcept -> void {
            clk->put(0);
            sleep_us(1);                                                                //REPLACE: with native sleep_us function
            clk->put(1);
        } 

        /// @brief Powers up the HX711 chip so it becomes operational again
        auto power_up() noexcept -> void {
            clk->put(0);
        }


    private:

        GPIO dout;
        GPIO clk;
        uint8_t gain;
        long offset; 
        float scale;

        [[nodiscard]] auto shift_in() noexcept -> uint8_t {
            uint8_t value = 0;

            for(uint8_t i = 0; i < 8; i++) {
                clk->put(1);
                sleep_us(1);                                                            //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
                
                value |= dout->get() << (7 - i); //most significant bit first

                clk->put(0);
                sleep_us(1);                                                            //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
            }

            return value;
        }

        [[noddiscard]] auto read() noexcept -> long {
            
            wait_ready();

            unsigned long value = 0;
            uint8_t data[3] = { 0 };
            uint8_t filler = 0x00;
            
            //IMPORTANT: add a interrupt disable here because this wil ruin the data transfer (freertos: task enter critrial section)

            //pulse the clock 24 times to get the data out
            data[2] = shift_in();
            data[1] = shift_in();
            data[0] = shift_in();

            //set the next gain / channel
            for( uint8_t i = 0; i < gain; i++) {
                clk->put(1);
                sleep_us(1);                                                            //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
                clk->put(0);
                sleep_us(1);                                                            //REPLACE: with native sleep_us function IF you work with a fast uC (pico 2 teensy esp32 etc) ELSE remove
            }

            //IMPORTANT: add a interrupt enable here to reenable the interupts (freertos: task exit critrial section)

            // Replicate the most significant bit to pad out a 32-bit signed integer
            if( data[2] & 0x80) {
                filler = 0xFF;
            }

            value = ( static_cast<unsigned long>(filler) << 24
                    | static_cast<unsigned long>(data[2]) << 16
                    | static_cast<unsigned long>(data[1]) << 8
                    | static_cast<unsigned long>(data[0]) );
            

            return static_cast<long>(value);
        }
};
