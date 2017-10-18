#pragma once

#include <string>
#include <mqtt/callback.h>
#include "../logger/log.h"

namespace BiosHomeAutomator {

  class Callback : public virtual mqtt::callback
  {
    public:
    	void connection_lost(const std::string& cause) override {
    		FILE_LOG(logWARNING) << "Connection lost";
    		if (!cause.empty()) {
          FILE_LOG(logWARNING) << "\tcause: " << cause;
        }
    	}

    	void delivery_complete(mqtt::delivery_token_ptr tok) override {
    		FILE_LOG(logVERBOSE) << "Delivery complete for token: "
          << (tok ? tok->get_message_id() : -1);
    	}
  };

};
