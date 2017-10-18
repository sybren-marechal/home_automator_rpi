#pragma once

#include <sstream>
#include <sys/time.h>

namespace BiosHomeAutomator {

  enum InputState {
    INPUT_HIGH, INPUT_LOW
  };

  enum StateChange {
    NONE, RISING, FALLING
  };

  class Input {
    private:
      unsigned int id;
      InputState currentState;
      InputState previousState;
      bool hasChanged;
      StateChange stateChange;

      struct timeval timeOfLastEvent;
      unsigned long millisecondsSincePreviousEvent;

    public:
      Input(unsigned int id, InputState currentState);

    public:
      unsigned int get_id(void);
      bool has_changed(void);
      unsigned long get_ms_since_previous_event(void);
      StateChange get_state_change(void);
      void set_current_state(InputState currentState);
      std::string to_string(void);

    private:
      void update_state_change(void);
      void update_timing(void);
  };

};
