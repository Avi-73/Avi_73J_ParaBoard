#pragma once

#include "utils/SensorData.h"

class ConditionChecker {
public:
  void begin();
  /*
   * 加速度による離床検知
   * 1kHzで呼ぶ
   */
  bool checkLaunchByAccel(AccelAndGyroData accelAndGyroData);
  /*
   * 気圧による離床検知
   * 25Hzで呼ぶ
   */
  bool checkLaunchByPressure(PressureData pressureData);
  /*
   * 気圧による頂点検知
   * 離床検知をしていないときはfalseを返す
   * 25Hzで呼ぶ
   */
  bool checkApogeeByPressure(PressureData pressureData);
  /*
   * タイマーによる頂点検知
   * 離床検知をしていないときはfalseを返す
   * 1kHzで呼ぶ
   */
  bool checkApogeeByTimer();
  /*
   * 離床検知をしているか
   */
  bool getIsLaunched();
  /*
   * 頂点検知をしているか
   */
  bool getHasReachedApogee();

private:
  bool isLaunched;
  bool hasReachedApogee;

  uint32_t launch_time;

  double accel_square_sum_for_check_launch;
  uint8_t pressure_decrease_count_for_check_launch;
  uint8_t pressure_increase_count_for_check_apogee;
  int16_t accel_data_count_for_check_launch;
  uint32_t pressure_sum_for_check_launch;
  uint32_t pressure_data_count_for_check_launch;
  uint32_t pressure_data_count_for_check_apogee;
  uint8_t last_pressure_for_check_apogee;
};