#pragma once

#include <bios_logger/logger.h>
#include <mqtt/iaction_listener.h>

namespace BiosHomeAutomator {

  class PublishActionListener : public virtual mqtt::iaction_listener
  {
    protected:
    	void on_failure(const mqtt::token& tok) override {
        BiosLogger::DoLog.warning("Listener failure for token: " + tok.get_message_id());
    	}

    	void on_success(const mqtt::token& tok) override {
    		BiosLogger::DoLog.verbose("Listener success for token: " + tok.get_message_id());
    	}
  };

};
