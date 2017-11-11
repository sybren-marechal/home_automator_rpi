#pragma once

#include <vector>
#include "../hal/io_relay_card.h"
#include "events/event.h"
#include "../mqtt/mqtt_channel.h"
#include <simple_mqtt_client/simple_mqtt_client.h>

namespace BiosHomeAutomator {

  class HomeAutomator : public BiosSimpleMqttClient::IMQTTMessageHandler {

    private:
      std::vector<IORelayCard *> relayCards;
      MQTTChannel * mqttChannel;

    public:
      HomeAutomator(MQTTChannel * mqttChannel);
      virtual ~HomeAutomator(void);

    public:
      void add_card(IORelayCard * relayCard);
      void process_changed_inputs(void);

    public:
      void handle_mqtt_message(BiosSimpleMqttClient::MQTTMessage mqttMessage) override;

    private:
      void process_event(Event * event);

  };

};
