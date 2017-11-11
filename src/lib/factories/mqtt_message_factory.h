#pragma once

#include <sstream>

namespace BiosHomeAutomator {

  class MQTTMessageFactory {

    private:
      const static int QOS = 1;

    public:
      static BiosSimpleMqttClient::MQTTMessage * create_input_state_update(Input * input) {
        std::stringstream ss;
        ss << "home/cards/" << input->get_parent()->get_id()
          << "/inputs/" << input->get_id();

        std::string topic = ss.str();

        // This does not actually belong here. Needs refactor
        std::string payload = "";
        // if (input->get_state_change() == FALLING) {
          if (input->get_ms_since_previous_event() > 1000 && input->get_ms_since_previous_event() < 3000) {
            payload = "LONG_PRESS";
          } else if (input->get_ms_since_previous_event() <= 1000) {
            payload = "SHORT_PRESS";
          } else {
            return nullptr;
          }
        // } else {
        //   return nullptr;
        // }

        BiosSimpleMqttClient::MQTTMessage * message = new BiosSimpleMqttClient::MQTTMessage(topic, payload);
        return message;
      }

  };

};
