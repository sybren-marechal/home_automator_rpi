#pragma once

#include "event.h"
#include "../../hal/io_relay_card.h"

namespace BiosHomeAutomator {

  enum RelayEventType {
    ON,
    OFF,
    TOGGLE
  };

  class RelayEvent : public Event {

    private:
      unsigned int relayId;
      RelayEventType type;

    public:
      RelayEvent(unsigned int expansionCardId, unsigned int relayId, RelayEventType type)
        : Event(expansionCardId) {

        this->relayId = relayId;
        this->type = type;
      }

      virtual void apply_event_on(ExpansionCard * card) override {
        if (card->get_id() == this->get_expansion_card_id()) {
          switch(type) {
            case ON:
              ((IORelayCard*)card)->activate_relay(relayId);
              break;
            case OFF:
              ((IORelayCard*)card)->deactivate_relay(relayId);
              break;
            case TOGGLE:
              ((IORelayCard*)card)->toggle_relay(relayId);
              break;
          }
        } else {
          FILE_LOG(logWARNING) << "Trying to apply event on wrong expansion card";
        }
      }

  };

};
