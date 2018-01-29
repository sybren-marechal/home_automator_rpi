#pragma once

#include "../home_automator_config.h"

namespace BiosHomeAutomator {

  // Cannot be part of the class, needs to be global function for nlohmann/json lib
  // void from_json(const json& jsonConfig, RestLoggerConfig& config) {
  //   config->set_ip(jsonConfig.at("ip").get<std::string>());
  //   config->set_port(jsonConfig.at("port").get<int>());
  //   config->set_auth_key(jsonConfig.at("auth_key").get<std::string>());
  // }

  void from_json(const json& jsonConfig, MQTTConfig& config) {
    config->set_server(jsonConfig.at("server").get<std::string>());
    config->set_client_id(jsonConfig.at("client_id").get<std::string>());
  }

  void from_json(const json& jsonConfig, HomeAutomatorConfig& config) {
    MQTTConfig mqttConfig = jsonConfig.at("mqtt");    // Try catch?
    config->set_mqtt_config(mqttConfig);
  }

};
