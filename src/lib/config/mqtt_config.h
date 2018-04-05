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
      std::string get_server(void) const;
      std::string get_client_id(void) const;

    public:
      void set_server(std::string server);
      void set_client_id(std::string clientId);
  };

};
