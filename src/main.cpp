#include <iostream>
#include <unistd.h>
#include "lib/home_automator/home_automator.h"
#include "lib/logger/log.h"

using namespace std;
using namespace BiosHomeAutomator;

const std::string MQTT_SERVER_ADDRESS	{ "tcp://10.0.0.100:1883" };
const std::string MQTT_CLIENT_ID		{ "sdfasdt54654hg4fh5334543" };

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

  MQTTChannel * mqttChannel = new MQTTChannel(MQTT_SERVER_ADDRESS, MQTT_CLIENT_ID);
  mqttChannel->connect();

  automator = new HomeAutomator(mqttChannel);
  automator->add_card(new IORelayCard(0x20, 1));

  FILE_LOG(logINFO) << "All ready for action ...";

  while (true) {
    sleep(1);
  }

  return 0;
}
