#include "callback.h"
#include "../logger/log.h"
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
			FILE_LOG(logERROR) << "MQTT Client connection error: " << exc.what();
			exit(1);
		}
	}

	// Re-connection failure
	void Callback::on_failure(const mqtt::token& tok) {
		FILE_LOG(logWARNING) << "Connection to mqtt broker failed";
		if (++numberOfConnectionRetries > N_RETRY_ATTEMPTS) {
      FILE_LOG(logERROR) << "Could not connect with broker";
			exit(1);
    }
		reconnect();
	}

	// Re-connection success
	void Callback::on_success(const mqtt::token& tok) {
		FILE_LOG(logINFO) << "Connection to mqtt broker success";
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void Callback::connection_lost(const std::string& cause) {
		FILE_LOG(logWARNING) << "Connection to mqtt broker lost";
		if (!cause.empty()) {
			FILE_LOG(logWARNING) << "\tcause: " << cause;
    }

		FILE_LOG(logWARNING) << "Reconnecting to mqtt broker ...";
		numberOfConnectionRetries = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void Callback::message_arrived(mqtt::const_message_ptr msg) {
		FILE_LOG(logVERBOSE) << "Message arrived with "
		  << "topic: '" << msg->get_topic() << "'"
  		<< " and payload: '" << msg->to_string() << "'";

    if (processor) {
      processor->process_incoming_message(msg);
    } else {
      FILE_LOG(logWARNING) << "No message processor registered";
    }
	}

	void Callback::delivery_complete(mqtt::delivery_token_ptr token) {}

  void Callback::register_message_processor(IMQTTMessageProcessor * processor) {
    this->processor = processor;
  }

};
