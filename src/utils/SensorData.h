#pragma once

#include <Arduino.h>

struct AccelAndGyroData {
  int16_t accel[3];
  int16_t gyro[3];
};

struct PressureData {
  uint32_t pressure;
};

struct LogData {
  uint32_t timestamp;
  AccelAndGyroData accelAndGyroData;
  PressureData pressureData;
};