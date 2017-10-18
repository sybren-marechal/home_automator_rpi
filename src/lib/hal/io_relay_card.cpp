#include "io_relay_card.h"

namespace BiosHomeAutomator {

  IORelayCard::IORelayCard(unsigned int ioExpanderAddress, unsigned int id)
    : ExpansionCard(id), ioExpander(ioExpanderAddress, "/dev/i2c-1") {

    // Configure outputs and set outputs low
    unsigned int mask = 0x03FF;
    ioExpander.set_port_direction(mask);
    all_relays_off();

    initialize_inputs();
  }

  void IORelayCard::initialize_inputs(void) {
    unsigned int data = ioExpander.read_port();
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      Input * input = new Input(i, determine_input_state(data, i));
      inputs.push_back(input);
    }
  }

  IORelayCard::~IORelayCard(void) {
    for (unsigned int i = 0; i < inputs.size(); i++) {
      delete inputs[i];
    }
    inputs.clear();
  }

  void IORelayCard::update_inputs(void) {
    unsigned int data = ioExpander.read_port();
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      inputs[i]->set_current_state(determine_input_state(data, inputs[i]->id));
    }
  }

  InputState IORelayCard::determine_input_state(unsigned int portState, unsigned int inputId) {
    if ((portState & (0x0001 << inputId)) != 0) {
      return INPUT_HIGH;
    } else {
      return INPUT_LOW;
    }
  }

  std::vector<Input*> IORelayCard::get_changed_inputs(void) {
    update_inputs();
    std::vector<Input*> changed;
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      if (inputs[i]->hasChanged) {
        changed.push_back(inputs[i]);
      }
    }
    return changed;
  }

  void IORelayCard::activate_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToSet = (0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData |= bitToSet;
      ioExpander.write_port(outputData);
    }
  }

  void IORelayCard::deactivate_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToClear = ~(0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData &= bitToClear;
      ioExpander.write_port(outputData);
    }
  }

  void IORelayCard::toggle_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToToggle = (0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData ^= bitToToggle;
      ioExpander.write_port(outputData);
    }
  }

  void IORelayCard::all_relays_off(void) {
    outputData = 0x0000;
    ioExpander.write_port(outputData);
  }

};
