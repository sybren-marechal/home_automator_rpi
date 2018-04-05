#pragma once

#include "mqtt_config.h"

namespace BiosHomeAutomator {

  class HomeAutomatorConfig {
    private:
      MQTTConfig mqttConfig;

    public:
      HomeAutomatorConfig(void);

    public:
      MQTTConfig get_mqtt_config(void) const;
      void set_mqtt_config(MQTTConfig config);
  };

};
