#include "callback.h"
#include <bios_logger/logger.h>
// #include <iostream>
// #include <cstdlib>
// #include <string>
// #include <cstring>
// #include <cctype>
// #include <thread>
// #include <chrono>

namespace BiosHomeAutomator {

  Callback::Callback(mqtt::async_client& mqttClient, mqtt::connect_options& connOpts)
    : client(mqttClient), connectionOptions(connOpts) {
    numberOfConnectionRetries = 0;
    processor = nullptr;
  }

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void Callback::reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			client.connect(connectionOptions, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
      BiosLogger::DoLog.error("MQTT Client connection error: " + std::string(exc.what()));
			exit(1);
		}
	}

	// Re-connection failure
	void Callback::on_failure(const mqtt::token& tok) {
    BiosLogger::DoLog.warning("Connection to mqtt broker failed");
		if (++numberOfConnectionRetries > N_RETRY_ATTEMPTS) {
      BiosLogger::DoLog.error("Could not connect with broker");
			exit(1);
    }
		reconnect();
	}

	// Re-connection success
	void Callback::on_success(const mqtt::token& tok) {
    BiosLogger::DoLog.info("Connection to mqtt broker success");
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void Callback::connection_lost(const std::string& cause) {
    BiosLogger::DoLog.warning("Connection to mqtt broker lost");
		if (!cause.empty()) {
      BiosLogger::DoLog.warning("\tcause: " + cause);
    }

    BiosLogger::DoLog.warning("Reconnecting to mqtt broker ...");
		numberOfConnectionRetries = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void Callback::message_arrived(mqtt::const_message_ptr msg) {
    BiosLogger::DoLog.verbose("Message arrived with topic: '"
      + std::string(msg->get_topic()) + "' and payload: '"
      + msg->to_string() + "'");

    if (processor) {
      processor->process_incoming_message(msg);
    } else {
      BiosLogger::DoLog.warning("No message processor registered");
    }
	}

	void Callback::delivery_complete(mqtt::delivery_token_ptr token) {}

  void Callback::register_message_processor(IMQTTMessageProcessor * processor) {
    this->processor = processor;
  }

};
