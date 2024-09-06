#pragma once

#include <Arduino.h>

/**
 * 加速度、各速度データ
 * 変換後のデータを入れる
 */
struct AccelAndGyroData {
  double accel[3];
  double gyro[3];
};

/**
 * 気圧データ
 * 変換後のデータを入れる
 */
struct PressureData {
  uint32_t pressure;
};

struct LogData {
  uint32_t timestamp;
  AccelAndGyroData accelAndGyroData;
  PressureData pressureData;
};