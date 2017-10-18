#pragma once

#include <mqtt/iaction_listener.h>

namespace BiosHomeAutomator {

  class PublishActionListener : public virtual mqtt::iaction_listener
  {
    protected:
    	void on_failure(const mqtt::token& tok) override {
    		FILE_LOG(logVERBOSE) << "Listener failure for token: " << tok.get_message_id();
    	}

    	void on_success(const mqtt::token& tok) override {
    		FILE_LOG(logVERBOSE) << "Listener success for token: " << tok.get_message_id();
    	}
  };

};
