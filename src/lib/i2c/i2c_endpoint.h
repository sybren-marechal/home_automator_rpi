#pragma once

#include <string>

namespace BiosHomeAutomator {

  class I2cEndpoint {

    private:
      std::string filename;
      unsigned int address;
      int fileHandle;

    public:
      I2cEndpoint(unsigned int address, std::string filename);

    public:
      bool open(void);
      bool write(char * data, int length);
      bool read(char * buffer, int length);
  };

}
