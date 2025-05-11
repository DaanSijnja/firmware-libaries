#pragma once

/* 
 *  TODO'S
 *     - write documentation for all functions
 *     - implement VISEN later 
 */

namespace mp6550
{
    /// @brief 
    /// @tparam GPIO 
    /// @tparam PWM 
    template<class GPIO, class PWM>
    class mp6550 {
        public:
            
            /// @brief 
            /// @param in_p 
            /// @param in_n 
            /// @param sleep_pin 
            /// @param max_cycle 
            explicit constexpr mp6550(
                PWM in_p,
                PWM in_n,
                GPIO sleep_pin,
                uint max_cycle
            ) noexcept :
                in_p(in_p),
                in_n(in_n),
                sleep_pin(sleep_pin),
                max_cycle(max_cycle)
            {} 

            /// @brief 
            auto init() noexcept -> void {

                in_p->init_gpio();
                in_n->init_gpio();

                in_p->set_enabled(true);
                in_n->set_enabled(true);

                in_p->set_wrap(max_cycle);
                in_n->set_wrap(max_cycle);

                sleep_pin->set_dir(true);
                sleep_pin->put(0);
            }

            /// @brief 
            /// @param percentage 
            auto set_percentage(float percentage) noexcept -> void {
                
                int value = ( percentage / 100.0 ) * max_cycle;
                set_value(value);
            }


            /// @brief 
            /// @param value 
            /// @return 
            auto set_value(int value) noexcept -> void {
                
                sleep(false);

                if(value > max_cycle) { value = max_cycle; }
                if(value < -max_cycle) { value = -max_cycle; }

                if( value < 0) {
                    in_p->set_level(0);    
                    in_n->set_level(-value);
                }

                if( value > 0) {
                    in_p->set_level(value);    
                    in_n->set_level(0);
                }

                if( value == 0 ) {
                    in_p->set_level(0);  
                    in_n->set_level(0);      
                }

            }

            /// @brief 
            /// @param to_sleep 
            /// @return 
            auto stop(bool to_sleep = false) noexcept -> void {
                set_value(0);
                sleep(to_sleep);
            }

            /// @brief 
            /// @param value 
            /// @return 
            auto sleep(bool value) noexcept -> void {
                if(value){
                    sleep_pin->put(0);
                } else {
                    sleep_pin->put(1);
                }
            }

        private:
            PWM in_p;
            PWM in_n;   

            GPIO sleep_pin;

            int max_cycle;   
    };
}



