#pragma once

#include <string>
#include <mqtt/async_client.h>
#include "callback.h"
#include "../multithread/safe_queue.h"
#include "listeners/subscribe_action_listener.h"
#include "i_mqtt_message_processor.h"

namespace BiosHomeAutomator {

  class MQTTChannel {
    private:
      const int	QOS = 1;

    private:
    	mqtt::async_client client;
      mqtt::connect_options connectionOptions;
      Callback * callback;
      SubscribeActionListener subscriptionListener;

      // Threading stuff
      bool keepPublishing;
      std::thread publisherThread;
      SafeQueue<mqtt::message_ptr> queue;

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

  };

};
