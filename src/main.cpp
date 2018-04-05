#include <unistd.h>   // For sleep
#include "lib/home_automator/home_automator.h"
#include <bios_logger/logger.h>
#include <bios_logger/writers/terminal_log_writer.h>
#include <bios_logger/writers/remote_rest_log_writer.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "lib/config/mqtt_config.h"
#include "lib/config/home_automator_config.h"
#include "lib/config/json/to_from_json.h"

using namespace std;
using namespace BiosHomeAutomator;
using namespace BiosLogger;

const std::string MQTT_SERVER_ADDRESS 	{ "tcp://10.0.0.100:1883" };
const std::string MQTT_CLIENT_ID		    { "sdfasdt54654hg4fh5334543" };
const std::string REST_LOGGER_HOST		  { "10.0.0.100" };
const int REST_LOGGER_PORT              = 3000;
const std::string REST_LOGGER_AUTH_KEY  { "rwfdh4e2" };
const std::string VERSION               { "version 1.1.0" };

// Don't know how to fix this yet
HomeAutomator * automator;
void relay_card_interrupt_handler(void) {
  if (automator) {
    automator->process_changed_inputs();
  }
}

int main(void) {

  // Read config from json file
  std::ifstream inputStream("/home/pi/share/home_automator_relay_card_rpi_rebuild/bin/test.json");
  nlohmann::json jsonConfig;
  inputStream >> jsonConfig;

  HomeAutomatorConfig config = jsonConfig;
  MQTTConfig mqttConfig = config.get_mqtt_config();

  cout << "MQTT Config - server: " << mqttConfig.get_server() << endl;
  cout << "MQTT Config - clientId: " << mqttConfig.get_client_id() << endl;


  // DoLog.register_log_writer(new RemoteRestLogWriter(REST_LOGGER_AUTH_KEY, "/messages.json", REST_LOGGER_HOST, REST_LOGGER_PORT, logVERBOSE));
  // DoLog.register_log_writer(new TerminalLogWriter(logVERBOSE));
  // DoLog.info("Starting Home Automator ...");
  // DoLog.info("Current version: " + VERSION);

  // // MQTTChannel * mqttChannel = new MQTTChannel(MQTT_SERVER_ADDRESS, MQTT_CLIENT_ID);
  // MQTTChannel * mqttChannel = new MQTTChannel(config.get_mqtt_config().get_server(), config.get_mqtt_config().get_client_id());

  // automator = new HomeAutomator(mqttChannel);
  // automator->add_card(new IORelayCard(0x20, 0));

  // DoLog.info("All ready for action ...");

  // while (true) {
  //   sleep(1);
  // }

  return 0;
}
