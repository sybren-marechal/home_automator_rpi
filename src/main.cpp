#include <stdio.h>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <bitset>
#include "lib/drivers/pcf8575.h"
#include <thread>         // std::thread
#include <time.h>         /* time */
#include <vector>
#include "lib/hal/io_relay_card.h"

#define BUTTON_PIN 1

using namespace std;
using namespace BiosHomeAutomator;

IORelayCard * relayCard;

void relayCardInputChanged(void) {
  vector<Input*> inputs = relayCard->get_changed_inputs();
  for (unsigned int i = 0; i < inputs.size(); i++) {
    cout << inputs[i]->to_string() << endl;
  }
}

int main(void) {
  if (wiringPiSetup () < 0) {
			std::cout << "Unable to setup wiringPi" << std::endl;
      return 1;
  }

	relayCard = new IORelayCard(0x20, 1);

  // Interrupt of io expander is falling edge
  if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &relayCardInputChanged) < 0) {
			std::cout << "Unable to setup ISR" << std::endl;
      return 1;
  }

  while (true) {
    sleep(1);
  }

	delete relayCard;
  return 0;
}
