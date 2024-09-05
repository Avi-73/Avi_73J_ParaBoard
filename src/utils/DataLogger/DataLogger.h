#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include "utils/SensorData.h"
#include "utils/SensorManager/SensorManager.h"
#include <SdFat.h>

class DataLogger {
public:
  void begin();
  void logData(const LogData &data);
  void end();

private:
  SdFat sd;
  SdFile file;
  char filename[20];
};

#endif