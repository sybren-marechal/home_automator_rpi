#include "pcf8575.h"

#include <iostream>
#include <unistd.h>   // For usleep
#include "../logger/log.h"

namespace BiosHomeAutomator {

  PCF8575::PCF8575(unsigned int address, std::string filename)
    : i2cEndpoint(address, filename) {

    if (!i2cEndpoint.open()) {    // Later exception here
      FILE_LOG(logERROR) << "Failed to open device " << filename << " with address = " << address;
    } else {
      set_all_as_inputs();
      write_port(0xFFFF);
      // usleep(100000);      // Needed or not?
    }
  }

  unsigned int PCF8575::read_port(void) {
    char buffer[2];
    if (!i2cEndpoint.read(buffer, sizeof(buffer))) {
      FILE_LOG(logWARNING) << "Read failed for pcf8575";
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
      FILE_LOG(logWARNING) << "Write failed for pcf8575";
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
