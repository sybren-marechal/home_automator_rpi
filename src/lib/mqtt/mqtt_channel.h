#pragma once

#include <simple_mqtt_client/simple_mqtt_client.h>
#include "../multithread/safe_queue.h"

namespace BiosHomeAutomator {

  class MQTTChannel {

    private:
      BiosSimpleMqttClient::SimpleMQTTClient simpleClient;

      // Threading stuff
      bool keepPublishing;
      std::thread publisherThread;
      SafeQueue<BiosSimpleMqttClient::MQTTMessage> queue;
      bool isPublisherThreadRunning;    // should be atomic actually

    public:
      MQTTChannel(std::string serverAddress, std::string clientId);
      virtual ~MQTTChannel(void);

    public:
      void publish(BiosSimpleMqttClient::MQTTMessage message);
      void subscribe(std::string topic, BiosSimpleMqttClient::IMQTTMessageHandler * handler);

    private:
      void start_publishing_thread(void);
      void stop_publishing_thread(void);
      void publishing_thread(void);

  };

};
