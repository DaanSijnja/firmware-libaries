#pragma once

namespace statemachine {

    /// @brief Abstract state class
    class base_state
    {
    public:
        /// @brief enter state function, is called when the state is entered
        /// @param stm void* | pointer to the statemachine this state is used in.
        virtual void enter(void* stm) = 0;

        /// @brief update state function, is called when the update is called
        /// @param stm void* | pointer to the statemachine this state is used in.
        virtual void update(void* stm) = 0;

        /// @brief exit state function, is called when the state is exited
        /// @param stm void* | pointer to the statemachine this state is used in.
        virtual void exit(void* stm) = 0;

        virtual ~base_state() {}

    };

}