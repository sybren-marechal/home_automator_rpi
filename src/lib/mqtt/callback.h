#pragma once

#include <string>
// #include <mqtt/callback.h>
// #include <mqtt/iaction_listener.h>
#include <mqtt/async_client.h>
#include "i_mqtt_message_processor.h"

namespace BiosHomeAutomator {

  class Callback : public virtual mqtt::callback,
  					public virtual mqtt::iaction_listener
  {
    private:
      const int	N_RETRY_ATTEMPTS = 5;

    private:
    	int numberOfConnectionRetries;
    	mqtt::async_client& client;
    	mqtt::connect_options& connectionOptions;
      IMQTTMessageProcessor * processor;

    public:
      Callback(mqtt::async_client& mqttClient, mqtt::connect_options& connOpts);

    public:
    	void on_failure(const mqtt::token& tok) override;
    	void on_success(const mqtt::token& tok) override;
    	void connection_lost(const std::string& cause) override;
    	void message_arrived(mqtt::const_message_ptr msg) override;
    	void delivery_complete(mqtt::delivery_token_ptr token) override;

    public:
      void register_message_processor(IMQTTMessageProcessor * processor);

    private:
    	void reconnect();
  };

};
