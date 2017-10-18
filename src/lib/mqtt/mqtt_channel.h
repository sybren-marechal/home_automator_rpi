#pragma once

#include <string>
#include <mqtt/async_client.h>
#include "callback.h"
#include "../multithread/safe_queue.h"

namespace BiosHomeAutomator {

  class MQTTChannel {

    private:
    	mqtt::async_client client;
      mqtt::connect_options connectionOptions;
      Callback * callback;

      // Threading stuff
      bool keepPublishing;
      std::thread publisherThread;
      SafeQueue<mqtt::message_ptr> queue;

    public:
      MQTTChannel(std::string serverAddress, std::string clientId);

    public:
      void connect(void);
      void disconnect(void);

    public:
      void publish(mqtt::message_ptr message);

    private:
      void publishing_thread(void);

  };

};
