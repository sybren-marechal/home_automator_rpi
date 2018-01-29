#pragma once

#include <string>

namespace BiosHomeAutomator {

  class MQTTConfig {
    private:
      std::string server;
      std::string clientId;

    public:
      MQTTConfig(void);
      MQTTConfig(std::string server, std::string clientId);

    public:
      std::string get_server(void);
      std::string get_client_id(void);
  };

};
