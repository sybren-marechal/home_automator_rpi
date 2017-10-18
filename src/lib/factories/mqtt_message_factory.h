#pragma once

#include <sstream>

namespace BiosHomeAutomator {

  class MQTTMessageFactory {

    private:
      const static int QOS = 1;

    public:
      static mqtt::message_ptr create_input_state_update(Input * input) {
        std::stringstream ss;
        ss << "test/home/cards/" << input->get_parent()->get_id()
          << "/inputs/" << input->get_id();

        std::string topic = ss.str();

        // This does not actually belong here. Needs refactor
        std::string payload = "";
        if (input->get_state_change() == FALLING) {
          if (input->get_ms_since_previous_event() > 1000) {
            payload = "LONG_PRESS";
          } else {
            payload = "SHORT_PRESS";
          }
        } else {
          return nullptr;
        }

        mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
        pubmsg->set_qos(QOS);

        return pubmsg;
      }

  };

};
