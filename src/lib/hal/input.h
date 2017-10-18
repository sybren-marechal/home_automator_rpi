#pragma once

#include <sstream>

namespace BiosHomeAutomator {

  enum InputState {
    INPUT_HIGH, INPUT_LOW
  };

  enum StateChange {
    NONE, RISING, FALLING
  };

  class Input {
    public:
      unsigned int id;
      InputState currentState;
      InputState previousState;
      bool hasChanged;
      StateChange stateChange;

    public:
      Input(unsigned int id, InputState currentState) {
        this->id = id;
        this->previousState = this->currentState = currentState;
        hasChanged = false;
        stateChange = NONE;
      }

      void set_current_state(InputState currentState) {
        previousState = this->currentState;
        this->currentState = currentState;
        hasChanged = (this->previousState != this->currentState);
        update_state_change();
      }

      std::string to_string(void) {
        std::stringstream ss;
        ss << "Input " << id << " is " << (currentState == INPUT_LOW ? "LOW" : "HIGH");
        if (hasChanged) {
          ss << " changed from " << (previousState == INPUT_LOW ? "LOW" : "HIGH");
          ss << " = " << (stateChange == RISING ? "RISING" : "FALLING");
        }
        return ss.str();
      }

      private:
        void update_state_change(void) {
          if (!hasChanged) {
            stateChange = NONE;
          } else if (this->currentState == INPUT_HIGH) {
            stateChange = RISING;
          } else if (this->currentState == INPUT_LOW) {
            stateChange = FALLING;
          }
        }
  };

};
