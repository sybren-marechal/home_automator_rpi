#pragma once

#include <mqtt/iaction_listener.h>
#include <bios_logger/logger.h>

namespace BiosHomeAutomator {

  class SubscribeActionListener : public virtual mqtt::iaction_listener
  {
    protected:
    	void on_failure(const mqtt::token& tok) override {
        BiosLogger::DoLog.warning("Listener failure for token: " + tok.get_message_id());
    	}

    	void on_success(const mqtt::token& tok) override {
        BiosLogger::DoLog.verbose("Listener success for token: " + tok.get_message_id());
    		auto top = tok.get_topics();
    		if (top && !top->empty()) {
          BiosLogger::DoLog.verbose("\ttoken topic: '" + (*top)[0] + "', ...");
        }
    	}
  };

};
