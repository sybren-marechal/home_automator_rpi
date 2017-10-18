#include "expansion_card.h"

namespace BiosHomeAutomator {

  ExpansionCard::ExpansionCard(unsigned int id) {
    this->id = id;
  }

  ExpansionCard::~ExpansionCard(void) { }

  unsigned int ExpansionCard::get_id(void) {
    return id;
  }

};
