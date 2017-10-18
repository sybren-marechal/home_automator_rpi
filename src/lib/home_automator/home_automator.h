#pragma once

#include <vector>
#include "../hal/io_relay_card.h"
#include "../mqtt/mqtt_channel.h"

namespace BiosHomeAutomator {

  class HomeAutomator {

    private:
      std::vector<IORelayCard *> relayCards;
      MQTTChannel * mqttChannel;

    public:
      HomeAutomator(MQTTChannel * mqttChannel);
      virtual ~HomeAutomator(void);

    public:
      void add_card(IORelayCard * relayCard);
      void process_changed_inputs(void);

  };

};
