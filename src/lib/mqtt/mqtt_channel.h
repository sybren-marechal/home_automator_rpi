#pragma once

#include <string>
#include <mqtt/async_client.h>
#include "../multithread/safe_queue.h"
#include "listeners/subscribe_action_listener.h"
#include "i_mqtt_message_processor.h"

namespace BiosHomeAutomator {

  class MQTTChannel : public virtual mqtt::callback,
  					public virtual mqtt::iaction_listener {
    private:
      const int	QOS = 1;
      const int	N_RETRY_ATTEMPTS = 10;

    private:
    	mqtt::async_client client;
      mqtt::connect_options connectionOptions;
      SubscribeActionListener subscriptionListener;
    	int numberOfConnectionRetries;
      IMQTTMessageProcessor * processor;

      // Threading stuff
      bool keepPublishing;
      std::thread publisherThread;
      SafeQueue<mqtt::message_ptr> queue;
      bool isPublisherThreadRunning;    // should be atomic actually

    public:
      MQTTChannel(std::string serverAddress, std::string clientId);

    public:
      void connect(void);
      void disconnect(void);
      void register_message_processor(IMQTTMessageProcessor * processor);

    public:
      void publish(mqtt::message_ptr message);

    private:
      void publishing_thread(void);
      void subscribe(void);

    public:
    	void on_failure(const mqtt::token& tok) override;
    	void on_success(const mqtt::token& tok) override;
    	void connection_lost(const std::string& cause) override;
    	void message_arrived(mqtt::const_message_ptr msg) override;
    	void delivery_complete(mqtt::delivery_token_ptr token) override;

    private:
    	void reconnect();

  };

};
