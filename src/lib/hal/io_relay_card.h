#pragma once

#include "expansion_card.h"
#include "../drivers/pcf8575.h"
#include <string>
#include <vector>
#include "input.h"

namespace BiosHomeAutomator {

  class IORelayCard : public ExpansionCard {

    private:
      static const unsigned int NUMBER_OF_INPUTS = 10;
      static const unsigned int NUMBER_OF_RELAYS = 6;

    private:
      PCF8575 ioExpander;
      unsigned int outputData;
      unsigned int id;
      std::vector<Input*> inputs;

    public:
      IORelayCard(unsigned int ioExpanderAddress, unsigned int id);
      virtual ~IORelayCard(void);

    public:
      void activate_relay(unsigned int relay);
      void deactivate_relay(unsigned int relay);
      void toggle_relay(unsigned int relay);
      void all_relays_off(void);
      std::vector<Input*> get_changed_inputs(void);

    private:
      void initialize_inputs(void);
      void update_inputs(void);
      InputState determine_input_state(unsigned int portState, unsigned int inputId);

  };

};
