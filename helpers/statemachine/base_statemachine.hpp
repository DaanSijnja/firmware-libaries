#pragma once

#include "base_state.hpp"

namespace statemachine {

    /// @brief Abstract statemachine class
    class base_statemachine
    {
        protected:
            base_state* current_state;

            /// @brief Get the current running state of the statemachine
            /// @return base_state* | base_state pointer to current state
            inline base_state* get_current_state() const {return current_state;}

            /// @brief Update current state
            /// @param stm void* | pointer to statemachine class, which is used in the state
            void state_update(void* stm) {
                current_state->update(stm);
            }

            /// @brief Sets a new state as the current state, it exits the current state and then enters the new state
            /// @param new_state base_state& | The state to set as current state
            /// @param stm void* | pointer to statemachine class, which is used in the state
            void set_state(base_state& new_state, void* stm) {
                current_state->exit(stm);
                current_state = &new_state;
                current_state->enter(stm);
            }
    };

}