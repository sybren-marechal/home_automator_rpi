#include <iostream>
#include <unistd.h>
#include "lib/home_automator/home_automator.h"

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
  cout << "Starting Home Automator ..." << endl;
  automator = new HomeAutomator();
  automator->add_card(new IORelayCard(0x20, 1));

  while (true) {
    sleep(1);
  }

  return 0;
}
