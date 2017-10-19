#pragma once

namespace BiosHomeAutomator {

  class Event {

    private:
      unsigned int expansionCardId;

    public:
      Event(unsigned int expansionCardId) {
        this->expansionCardId = expansionCardId;
      }

    public:
      unsigned int get_expansion_card_id(void) {
        return expansionCardId;
      }

    public:
      virtual void apply_event_on(ExpansionCard * card) = 0;

  };

};
