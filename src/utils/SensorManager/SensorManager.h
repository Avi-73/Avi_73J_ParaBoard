#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "utils/SensorData.h"
#include <Arduino.h>
#include <ICM20602.h>
#include <LPS25HB.h>
#include <SPICREATE.h>

class SensorManager {
public:
  void begin();
  AccelAndGyroData getAccelAndGyroData();
  PressureData getPressureData();

private:
  ICM icm;
  LPS lps;
  SPICREATE::SPICreate spi;
};

#endif
