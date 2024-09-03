#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <SPIflash.h>
#include "utils/SensorManager/SensorManager.h"

class DataLogger {
public:
  void begin();
  void logData(const SensorData& data);
  void retrieveData();

private:
  Flash flash;
  uint32_t currentAddress;
};

#endif