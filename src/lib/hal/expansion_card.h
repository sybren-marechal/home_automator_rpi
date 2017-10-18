#pragma once

namespace BiosHomeAutomator {

  class ExpansionCard {

    private:
      unsigned int id;

    public:
      ExpansionCard(unsigned int id);
      virtual ~ExpansionCard(void);

    public:
      unsigned int get_id(void);

  };

};
