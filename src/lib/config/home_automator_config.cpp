#include "home_automator_config.h"

namespace BiosHomeAutomator {

  HomeAutomatorConfig::HomeAutomatorConfig(void) {}

  MQTTConfig HomeAutomatorConfig::get_mqtt_config(void) const {
    return mqttConfig;
  }

  void HomeAutomatorConfig::set_mqtt_config(MQTTConfig config) {
    this->mqttConfig = config;
  }

};
