#include "pcf8575.h"

#include <string>
#include <bios_logger/logger.h>

namespace BiosHomeAutomator {

  PCF8575::PCF8575(unsigned int address, std::string filename)
    : i2cEndpoint(address, filename) {

    if (!i2cEndpoint.open()) {    // Later exception here
      BiosLogger::DoLog.error("Failed to open i2c endpoint on bus " + filename + " with address " + std::to_string(address));
    } else {
      set_all_as_inputs();
      write_port(0xFFFF);
    }
  }

  unsigned int PCF8575::read_port(void) {
    char buffer[2];
    if (!i2cEndpoint.read(buffer, sizeof(buffer))) {
      BiosLogger::DoLog.warning("Failed to read from PCF8575");
    };
    return ((buffer[1] << 8) | buffer[0]);
  }

  void PCF8575::write_port(unsigned int data) {
    // Inputs should stay 1, only outputs can change
    unsigned int output = data | directionMask;

    char buffer[2];
    buffer[0] = output;
    buffer[1] = (output >> 8);
    if (!i2cEndpoint.write(buffer, sizeof(buffer))) {
      BiosLogger::DoLog.warning("Failed to write to PCF8575");
    };
  }

  // Set 1 for inputs and 0 for outputs
  void PCF8575::set_port_direction(unsigned int mask) {
    this->directionMask = mask;
  }

  void PCF8575::set_all_as_inputs(void) {
    set_port_direction(ALL_INPUTS);
  }

};
