#pragma once

#include "../home_automator_config.h"
#include "../mqtt_config.h"
#include <nlohmann/json.hpp>

namespace BiosHomeAutomator {

  void from_json(const nlohmann::json& jsonConfig, MQTTConfig& config);
  void from_json(const nlohmann::json& jsonConfig, HomeAutomatorConfig& config);

};