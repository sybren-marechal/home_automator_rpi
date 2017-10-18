#pragma once

#include <vector>
#include "../hal/io_relay_card.h"

namespace BiosHomeAutomator {

  class HomeAutomator {

    private:
      std::vector<IORelayCard *> relayCards;

    public:
      HomeAutomator(void);
      virtual ~HomeAutomator(void);

    public:
      void add_card(IORelayCard * relayCard);
      void process_changed_inputs(void);

  };

};
