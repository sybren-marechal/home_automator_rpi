#include "mqtt_channel.h"
#include <bios_logger/logger.h>
#include <thread>
#include "listeners/publish_action_listener.h"

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(std::string serverAddress, std::string clientId)
    : client(serverAddress, clientId) {

    numberOfConnectionRetries = 0;
    processor = nullptr;
    isPublisherThreadRunning = false;

  	connectionOptions.set_keep_alive_interval(20);
  	connectionOptions.set_clean_session(true);

    client.set_callback(*this);
  }

  void MQTTChannel::connect(void) {
  	try {
      BiosLogger::DoLog.info("Connecting to the MQTT server ...");
  		client.connect(connectionOptions, nullptr, *this)->wait();
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
    // isPublisherThreadRunning = true;
    while(keepPublishing) {
      mqtt::message_ptr msg = queue.dequeue();
      BiosLogger::DoLog.info("Dequeuing message");
      PublishActionListener listener;
  		mqtt::delivery_token_ptr pubtok = client.publish(msg, nullptr, listener);
  		pubtok->wait();
    }
    // isPublisherThreadRunning = false;
  }

  void MQTTChannel::subscribe(void) {
    std::string topic = "home/cards/+/relays/+/set";
    BiosLogger::DoLog.info("Subscribing to topic '" + topic + "' using QoS" + std::to_string(QOS));
		client.subscribe(topic, QOS);
  }

  void MQTTChannel::register_message_processor(IMQTTMessageProcessor * processor) {
    this->processor = processor;
  }

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void MQTTChannel::reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		try {
      BiosLogger::DoLog.debug("Reconnecting to MQTT broker");
			client.connect(connectionOptions, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			BiosLogger::DoLog.error("MQTT Client connection error: " + std::string(exc.what()));
			exit(1);
		}
	}

	// Re-connection failure
	void MQTTChannel::on_failure(const mqtt::token& tok) {
		BiosLogger::DoLog.warning("Connection to mqtt broker failed");
		if (++numberOfConnectionRetries > N_RETRY_ATTEMPTS) {
      BiosLogger::DoLog.error("Could not connect with broker");
			exit(1);
    }
		reconnect();
	}

	// Re-connection success
	void MQTTChannel::on_success(const mqtt::token& tok) {
		BiosLogger::DoLog.info("MQTT Connection ready ...");

    if (!isPublisherThreadRunning) {
      // Create thread for publishing
      BiosLogger::DoLog.info("Creating publisher thread ...");
      keepPublishing = true;
      publisherThread = std::thread(&MQTTChannel::publishing_thread, this);
      isPublisherThreadRunning = true;

      subscribe();
    } else {
      BiosLogger::DoLog.warning("Publisher thread already running ...");
    }
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void MQTTChannel::connection_lost(const std::string& cause) {
		BiosLogger::DoLog.warning("Connection to mqtt broker lost");
		if (!cause.empty()) {
			BiosLogger::DoLog.warning("\tcause: " + cause);
    }

		BiosLogger::DoLog.warning("Reconnecting to mqtt broker ...");
		numberOfConnectionRetries = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void MQTTChannel::message_arrived(mqtt::const_message_ptr msg) {
		BiosLogger::DoLog.verbose("Message arrived with topic: '" + msg->get_topic()
      + "' and payload: '" + msg->to_string() + "'");

    if (processor) {
      processor->process_incoming_message(msg);
    } else {
      BiosLogger::DoLog.warning("No message processor registered");
    }
	}

	void MQTTChannel::delivery_complete(mqtt::delivery_token_ptr token) {}

};
