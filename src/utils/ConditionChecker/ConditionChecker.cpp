#include "ConditionChecker.h"
#include "utils/Config.h"
#include <Arduino.h>

void ConditionChecker::begin() {
  isLaunched = false;
  hasReachedApogee = false;
  launch_time = 0;
  pressure_decrease_count_for_check_launch = 0;
  pressure_increase_count_for_check_apogee = 0;
  accel_square_sum_for_check_launch = 0;
  accel_data_count_for_check_launch = 0;
  pressure_sum_for_check_launch = 0;
  pressure_data_count_for_check_launch = 0;
  pressure_data_count_for_check_apogee = 0;
  last_pressure_for_check_apogee = 0;
}

bool ConditionChecker::checkLaunchByAccel(AccelAndGyroData accelAndGyroData) {
  double accel_x = (double)accelAndGyroData.accel[0] / 2048;
  double accel_y = (double)accelAndGyroData.accel[1] / 2048;
  double accel_z = (double)accelAndGyroData.accel[2] / 2048;
  accel_square_sum_for_check_launch += accel_x * accel_x + accel_y * accel_y + accel_z * accel_z;
  if (accel_data_count_for_check_launch == NUMBER_OF_ACCEL_DATA_FOR_LAUNCH) {
    // 離床判断
    if (accel_square_sum_for_check_launch >= ACCEL_SQUARE_SUM_THRESHOLD) {
      isLaunched = true;
      launch_time = millis();
    }
    // データをリセット
    accel_square_sum_for_check_launch = 0;
    accel_data_count_for_check_launch = 0;
  }
  accel_data_count_for_check_launch++;
  return isLaunched;
}

bool ConditionChecker::checkLaunchByPressure(PressureData pressureData) {
  pressure_sum_for_check_launch += pressureData.pressure;
  if (pressure_data_count_for_check_launch == NUMBER_OF_PRESSURE_DATA_FOR_LAUNCH) {
    if (pressure_sum_for_check_launch <= PRESSURE_SUM_THRESHOLD_FOR_LAUNCH) {
      pressure_decrease_count_for_check_launch++;
    }
    if (pressure_decrease_count_for_check_launch >= PRESSURE_DECREASE_COUNT_THRESHOLD_FOR_LAUNCH) {
      isLaunched = true;
      launch_time = millis();
    }
    pressure_sum_for_check_launch = 0;
    pressure_data_count_for_check_launch = 0;
  }
  pressure_data_count_for_check_launch++;
  return isLaunched;
}

bool ConditionChecker::checkApogeeByPressure(PressureData pressureData) {
  if (!isLaunched) {
    return false;
  }
  if (pressure_data_count_for_check_apogee == NUMBER_OF_PRESSURE_DATA_FOR_APOGEE) {
    if (pressureData.pressure - last_pressure_for_check_apogee > 0) {
      pressure_increase_count_for_check_apogee++;
    }
    if (pressure_increase_count_for_check_apogee >= PRESSURE_INCREASE_COUNT_THRESHOLD_FOR_APOGEE) {
      hasReachedApogee = true;
    }
    last_pressure_for_check_apogee = pressureData.pressure;
    pressure_data_count_for_check_apogee = 0;
  }
  pressure_data_count_for_check_apogee++;
  return hasReachedApogee;
}

bool ConditionChecker::checkApogeeByTimer() {
  if (!isLaunched) {
    return false;
  }
  if (millis() - launch_time >= TIME_THRESHOLD_FOR_APOGEE_FROM_LAUNCH) {
    hasReachedApogee = true;
  }
  return hasReachedApogee;
}

bool ConditionChecker::getIsLaunched() { return isLaunched; }

bool ConditionChecker::getHasReachedApogee() { return hasReachedApogee; }