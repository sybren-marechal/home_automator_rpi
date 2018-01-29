#include "mqtt_config.h"

namespace BiosHomeAutomator {

  MQTTConfig::MQTTConfig(void)
    : MQTTConfig("tcp://broker.hivemq.com:1883", "none_what_so_ever") {
  }

  MQTTConfig::MQTTConfig(std::string server, std::string clientId) {
    this->server = server;
    this->clientId = clientId;
  }

  std::string MQTTConfig::get_server(void) {
    return this->server;
  }

  std::string MQTTConfig::get_client_id(void) {
    return this->clientId;
  }

};
