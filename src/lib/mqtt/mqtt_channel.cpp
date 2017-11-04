#include "mqtt_channel.h"
#include <bios_logger/logger.h>
#include <thread>
#include "listeners/publish_action_listener.h"

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(std::string serverAddress, std::string clientId)
    : client(serverAddress, clientId) {

  	connectionOptions.set_keep_alive_interval(20);
  	connectionOptions.set_clean_session(true);

    callback = new Callback(client, connectionOptions);
    client.set_callback(*callback);
  }

  void MQTTChannel::connect(void) {
  	try {
      BiosLogger::DoLog.info("Connecting to the MQTT server ...");
  		client.connect(connectionOptions, nullptr, *callback)->wait();
  		BiosLogger::DoLog.info("MQTT Connection ready ...");

      // Create thread for publishing
      BiosLogger::DoLog.info("Creating publisher thread ...");
      keepPublishing = true;
      publisherThread = std::thread(&MQTTChannel::publishing_thread, this);

      subscribe();
  	}
  	catch (const mqtt::exception&) {
  		BiosLogger::DoLog.error("Unable to connect to the MQTT server");
  	}
  }

  void MQTTChannel::disconnect(void) {
    keepPublishing = false;
    publisherThread.join();   // Wait for thread to finish

		auto toks = client.get_pending_delivery_tokens();
		if (!toks.empty()) {
      BiosLogger::DoLog.warning("There are pending MQTT delivery tokens!");
    }

  	try {
  		BiosLogger::DoLog.info("Disconnecting from MQTT server ...");
  		client.disconnect()->wait();
  		BiosLogger::DoLog.info("Disconnected from MQTT server");
  	}
  	catch (const mqtt::exception& exc) {
  		BiosLogger::DoLog.error("Exception: " + std::string(exc.what()));
  	}
  }

  void MQTTChannel::publish(mqtt::message_ptr message) {
    BiosLogger::DoLog.info("Enqueuing message");
    queue.enqueue(message);
  }

  void MQTTChannel::publishing_thread(void) {
    while(keepPublishing) {
      mqtt::message_ptr msg = queue.dequeue();
      BiosLogger::DoLog.info("Dequeuing message");
      PublishActionListener listener;
  		mqtt::delivery_token_ptr pubtok = client.publish(msg, nullptr, listener);
  		pubtok->wait();
    }
  }

  void MQTTChannel::subscribe(void) {
    std::string topic = "home/cards/+/relays/+/set";
		BiosLogger::DoLog.info("Subscribing to topic '" + topic + "' using QoS" + std::to_string(QOS));
		client.subscribe(topic, QOS, nullptr, subscriptionListener);
  }

  void MQTTChannel::register_message_processor(IMQTTMessageProcessor * processor) {
    callback->register_message_processor(processor);
  }

};
