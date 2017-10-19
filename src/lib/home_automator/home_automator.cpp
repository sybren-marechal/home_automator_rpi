#include "home_automator.h"

#include <iostream>
#include <vector>
#include <wiringPi.h>
#include "../logger/log.h"
#include "../factories/mqtt_message_factory.h"
#include "../factories/event_factory.h"

#define BUTTON_PIN 1
extern void relay_card_interrupt_handler(void);

namespace BiosHomeAutomator {

  HomeAutomator::HomeAutomator(MQTTChannel * mqttChannel) {
    this->mqttChannel = mqttChannel;
    this->mqttChannel->register_message_processor(this);

    if (wiringPiSetup () < 0) {
      // Should throw exception
      FILE_LOG(logERROR) << "Unable to setup wiringPi";
      return;
    }

    // Interrupt of io expander is falling edge
    if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, relay_card_interrupt_handler) < 0) {
      FILE_LOG(logERROR) << "Unable to setup ISR";
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
        FILE_LOG(logWARNING) << "FAIL: ID for expansion card already taken";
        return;
      }
    }

    relayCards.push_back(relayCard);
  }

  void HomeAutomator::process_changed_inputs(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      std::vector<Input*> inputs = relayCards[i]->get_changed_inputs();
      for (unsigned int i = 0; i < inputs.size(); i++) {
        FILE_LOG(logVERBOSE) << inputs[i]->to_string();

        mqtt::message_ptr message = MQTTMessageFactory::create_input_state_update(inputs[i]);
        if (message) {
          mqttChannel->publish(message);
        }
      }
    }
  }

  void HomeAutomator::process_incoming_message(mqtt::const_message_ptr message) {
    Event * event = EventFactory::create_event_from_mqtt_message(message);
    if (event) {
      process_event(event);
    }
  }

  void HomeAutomator::process_event(Event * event) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      if (relayCards[i]->get_id() == event->get_expansion_card_id()) {
        event->apply_event_on(relayCards[i]);
        return;
      }
    }
  }

};
