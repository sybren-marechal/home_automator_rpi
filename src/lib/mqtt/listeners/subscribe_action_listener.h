#pragma once

#include <mqtt/iaction_listener.h>
#include "../../logger/log.h"

namespace BiosHomeAutomator {

  class SubscribeActionListener : public virtual mqtt::iaction_listener
  {
    protected:
    	void on_failure(const mqtt::token& tok) override {
    		FILE_LOG(logWARNING) << "Listener failure for token: " << tok.get_message_id();
    	}

    	void on_success(const mqtt::token& tok) override {
    		FILE_LOG(logVERBOSE) << "Listener success for token: " << tok.get_message_id();
    		auto top = tok.get_topics();
    		if (top && !top->empty()) {
          FILE_LOG(logVERBOSE) << "\ttoken topic: '" << (*top)[0] << "', ...";
        }
    	}
  };

};
