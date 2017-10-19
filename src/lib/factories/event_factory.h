#pragma once

#include <sstream>
#include "../helpers/string_splitter.h"
#include "../home_automator/events/relay_event.h"

namespace BiosHomeAutomator {

  class EventFactory {

    public:
      static Event * create_event_from_mqtt_message(mqtt::const_message_ptr msg) {
        std::string topic =  msg->get_topic();
        std::string payload = msg->to_string();
        std::vector<std::string> topicParts = StringSplitter::split(topic, '/');
        // "home/cards/+/relays/+/set"

        Event * event = nullptr;

        if (topicParts.size() == 6 && topicParts[3] == "relays" && topicParts[5] == "set") {
          unsigned int cardId = std::stoi(topicParts[2]);   // Should catch the exceptions
          unsigned int relayId = std::stoi(topicParts[4]);
          RelayEventType eventType;

          if (payload == "ON") {
            eventType = ON;
          } else if (payload == "OFF") {
            eventType = OFF;
          } else if (payload == "TOGGLE") {
            eventType = TOGGLE;
          } else {
            FILE_LOG(logWARNING) << "Unknown event type in MQTT message: " << payload;
            return nullptr;
          }
          event = new RelayEvent(cardId, relayId, eventType);
        } else {
          FILE_LOG(logWARNING) << "Missing event topic parts in MQTT message: " << topic;
        }

        return event;
      }

  };

};
