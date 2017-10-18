#include "home_automator.h"

#include <iostream>
#include <vector>
#include <wiringPi.h>

#define BUTTON_PIN 1
extern void relay_card_interrupt_handler(void);

namespace BiosHomeAutomator {

  HomeAutomator::HomeAutomator(void) {
    if (wiringPiSetup () < 0) {
      // Should throw exception
			std::cout << "Unable to setup wiringPi" << std::endl;
      return;
    }

    // Interrupt of io expander is falling edge
    if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, relay_card_interrupt_handler) < 0) {
  		std::cout << "Unable to setup ISR" << std::endl;
      return;
    }
  }

  HomeAutomator::~HomeAutomator(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      delete relayCards[i];
    }
    relayCards.clear();
  }

  void HomeAutomator::add_card(IORelayCard * relayCard) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      if (relayCards[i]->get_id() == relayCard->get_id()) {
        std::cout << "FAIL: ID for expansion card already taken" << std::endl;
        return;
      }
    }

    relayCards.push_back(relayCard);
  }

  void HomeAutomator::process_changed_inputs(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      std::vector<Input*> inputs = relayCards[i]->get_changed_inputs();
      for (unsigned int i = 0; i < inputs.size(); i++) {
        std::cout << inputs[i]->to_string() << std::endl;
      }
    }
  }

};
