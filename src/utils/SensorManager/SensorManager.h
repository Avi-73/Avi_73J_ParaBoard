#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <ICM20602.h>
#include <LPS25HB.h>
#include <SPICREATE.h>

struct SensorData {
  int16_t accel[3];
  int16_t gyro[3];
  uint32_t pressure;
};

class SensorManager {
public:
  void begin();
  void triggerRead();
  bool dataReady();
  SensorData getData();

private:
  ICM icm;
  LPS lps;
  SPICREATE::SPICreate spi;
  SensorData latestData;
  bool _dataReady;
};

#endif