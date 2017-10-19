#pragma once

#include "mqtt/async_client.h"

namespace BiosHomeAutomator {

  class IMQTTMessageProcessor {

    public:
      virtual void process_incoming_message(mqtt::const_message_ptr msg) = 0;
  };

};
