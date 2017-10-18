#include <iostream>
#include <unistd.h>
#include "lib/home_automator/home_automator.h"
#include "lib/logger/log.h"

using namespace std;
using namespace BiosHomeAutomator;

// Don't know how to fix this yet
HomeAutomator * automator;
void relay_card_interrupt_handler(void) {
  if (automator) {
    automator->process_changed_inputs();
  }
}

int main(void) {
  FILELog::ReportingLevel() = FILELog::FromString("VERBOSE");

  FILE_LOG(logINFO) << "Starting Home Automator ...";
  automator = new HomeAutomator();
  automator->add_card(new IORelayCard(0x20, 1));

  while (true) {
    sleep(1);
  }

  return 0;
}
