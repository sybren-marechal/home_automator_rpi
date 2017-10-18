#include "input.h"

#include <sstream>

namespace BiosHomeAutomator {

  Input::Input(unsigned int id, InputState currentState) {
    this->id = id;
    this->previousState = this->currentState = currentState;
    hasChanged = false;
    stateChange = NONE;

    gettimeofday(&timeOfLastEvent, NULL);
    millisecondsSincePreviousEvent = 0;
  }

  void Input::set_current_state(InputState currentState) {
    previousState = this->currentState;
    this->currentState = currentState;
    hasChanged = (this->previousState != this->currentState);
    update_timing();
    update_state_change();
  }

  unsigned int Input::get_id(void) {
    return id;
  }

  bool Input::has_changed(void) {
    return hasChanged;
  }

  StateChange Input::get_state_change(void) {
    return stateChange;
  }

  unsigned long Input::get_ms_since_previous_event(void) {
    return millisecondsSincePreviousEvent;
  }

  std::string Input::to_string(void) {
    std::stringstream ss;
    ss << "Input " << id << " is " << (currentState == INPUT_LOW ? "LOW" : "HIGH");
    if (hasChanged) {
      ss << "\r\n\tchanged from " << (previousState == INPUT_LOW ? "LOW" : "HIGH");
      ss << " = " << (stateChange == RISING ? "RISING" : "FALLING");
      ss << "\r\n\t" << millisecondsSincePreviousEvent << "ms since last event";
    }
    return ss.str();
  }

  void Input::update_state_change(void) {
    if (!hasChanged) {
      stateChange = NONE;
    } else if (this->currentState == INPUT_HIGH) {
      stateChange = RISING;
    } else if (this->currentState == INPUT_LOW) {
      stateChange = FALLING;
    }
  }

  void Input::update_timing(void) {
    if (hasChanged) {
      struct timeval now;
      gettimeofday(&now, NULL);
      millisecondsSincePreviousEvent = ((now.tv_sec * 1000000 + now.tv_usec)
        - (timeOfLastEvent.tv_sec * 1000000 + timeOfLastEvent.tv_usec)) / 1000;
      timeOfLastEvent = now;
    }
  }

};
