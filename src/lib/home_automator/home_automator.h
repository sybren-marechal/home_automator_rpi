#pragma once

#include <vector>
#include "../hal/io_relay_card.h"
#include "../mqtt/mqtt_channel.h"
#include "../mqtt/i_mqtt_message_processor.h"
#include "events/event.h"

namespace BiosHomeAutomator {

  class HomeAutomator : public IMQTTMessageProcessor {

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
      void process_incoming_message(mqtt::const_message_ptr message) override;

    private:
      void process_event(Event * event);

  };

};
