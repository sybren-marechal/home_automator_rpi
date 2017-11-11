#pragma once

#include <string>
#include <bios_logger/logger.h>
#include "../helpers/string_splitter.h"
#include "../home_automator/events/relay_event.h"

namespace BiosHomeAutomator {

  class EventFactory {

    public:
      static Event * create_event_from_mqtt_message(BiosSimpleMqttClient::MQTTMessage mqttMessage) {
        std::string topic =  mqttMessage.get_topic();
        std::string payload = mqttMessage.get_message();
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
            BiosLogger::DoLog.warning("Unknown event type in MQTT message: " + payload);
            return nullptr;
          }
          event = new RelayEvent(cardId, relayId, eventType);
        } else {
          BiosLogger::DoLog.warning("Missing event topic parts in MQTT message: " + topic);
        }

        return event;
      }

  };

};
