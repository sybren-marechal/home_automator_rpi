#include "mqtt_channel.h"
#include <bios_logger/bios_logger.h>

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(std::string serverAddress, std::string clientId)
    : simpleClient(serverAddress, clientId) {

      keepPublishing = false;
      start_publishing_thread();
  }

  MQTTChannel::~MQTTChannel(void) {
    stop_publishing_thread();
  }

  void MQTTChannel::publish(BiosSimpleMqttClient::MQTTMessage message) {
    BiosLogger::DoLog.info("Enqueuing message " + message.get_message() + " @ " + message.get_topic());
    queue.enqueue(message);
  }

  void MQTTChannel::subscribe(std::string topic, BiosSimpleMqttClient::IMQTTMessageHandler * handler) {
		simpleClient.subscribe(topic, handler);
  }

  void MQTTChannel::start_publishing_thread(void) {
    keepPublishing = true;
    publisherThread = std::thread(&MQTTChannel::publishing_thread, this);
  }

  void MQTTChannel::stop_publishing_thread(void) {
    keepPublishing = false;
    publisherThread.join();   // Wait for thread to finish
  }

  void MQTTChannel::publishing_thread(void) {
    BiosLogger::DoLog.info("Starting publisher thread");
    while(keepPublishing) {
      BiosSimpleMqttClient::MQTTMessage msg = queue.dequeue();
      BiosLogger::DoLog.info("Dequeuing message " + msg.get_message() + " @ " + msg.get_topic());
      simpleClient.publish(msg);
      // TODO: Refactor with safe_thread library, sleep and empty check
    }
    BiosLogger::DoLog.info("Publisher thread finished");
  }

};
