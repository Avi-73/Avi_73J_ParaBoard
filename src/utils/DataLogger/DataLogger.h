#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include "utils/SensorData.h"
#include <SdFat.h>

class DataLogger {
public:
  void begin();
  void logData(const LogData &data);
  void end();

private:
  SdFat32 sd;
  File32 file;
  char filename[13];
  bool can_open_file;
};

#endif // DATA_LOGGER_H