#include "mqtt_channel.h"
#include "../logger/log.h"
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
  		FILE_LOG(logINFO) << "Connecting to the MQTT server ...";
  		client.connect(connectionOptions, nullptr, *callback)->wait();
  		FILE_LOG(logINFO) << "MQTT Connection ready ...";

      // Create thread for publishing
      FILE_LOG(logINFO) << "Creating publisher thread ...";
      keepPublishing = true;
      publisherThread = std::thread(&MQTTChannel::publishing_thread, this);

      subscribe();
  	}
  	catch (const mqtt::exception&) {
  		FILE_LOG(logERROR) << "Unable to connect to the MQTT server";
  	}
  }

  void MQTTChannel::disconnect(void) {
    keepPublishing = false;
    publisherThread.join();   // Wait for thread to finish

		auto toks = client.get_pending_delivery_tokens();
		if (!toks.empty()) {
      FILE_LOG(logWARNING) << "There are pending MQTT delivery tokens!";
    }

  	try {
  		FILE_LOG(logINFO) << "Disconnecting from MQTT server ...";
  		client.disconnect()->wait();
  		FILE_LOG(logINFO) << "Disconnected from MQTT server";
  	}
  	catch (const mqtt::exception& exc) {
  		FILE_LOG(logERROR) << exc.what();
  	}
  }

  void MQTTChannel::publish(mqtt::message_ptr message) {
    FILE_LOG(logINFO) << "Enqueuing message";
    queue.enqueue(message);
  }

  void MQTTChannel::publishing_thread(void) {
    while(keepPublishing) {
      mqtt::message_ptr msg = queue.dequeue();
      FILE_LOG(logINFO) << "Dequeuing message";
      PublishActionListener listener;
  		mqtt::delivery_token_ptr pubtok = client.publish(msg, nullptr, listener);
  		pubtok->wait();
    }
  }

  void MQTTChannel::subscribe(void) {
    std::string topic = "home/cards/+/relays/+/set";
		FILE_LOG(logINFO) << "Subscribing to topic '" << topic << "' using QoS" << QOS;
		client.subscribe(topic, QOS, nullptr, subscriptionListener);
  }

  void MQTTChannel::register_message_processor(IMQTTMessageProcessor * processor) {
    callback->register_message_processor(processor);
  }

};
