#include "ConditionChecker.h"
#include "utils/Config.h"
#include <Arduino.h>

void ConditionChecker::begin() {
  isLaunched = false;
  hasReachedApogee = false;
  launch_time = 0;
  pressure_decrease_count_for_check_launch = 0;
  pressure_increase_count_for_check_apogee = 0;
  accel_sum_for_check_launch[0] = 0;
  accel_sum_for_check_launch[1] = 0;
  accel_sum_for_check_launch[2] = 0;
  accel_data_count_for_check_launch = 0;
  pressure_sum_for_check_launch = 0;
  pressure_data_count_for_check_launch = 0;
  pressure_data_count_for_check_apogee = 0;
  last_pressure_for_check_apogee = 0;
}

bool ConditionChecker::checkLaunchByAccel(AccelAndGyroData accelAndGyroData) {
  // Serial.printf("accel: %.2f, %.2f, %.2f\n", accelAndGyroData.accel[0], accelAndGyroData.accel[1],
  //               accelAndGyroData.accel[2]);
  accel_sum_for_check_launch[0] += accelAndGyroData.accel[0];
  accel_sum_for_check_launch[1] += accelAndGyroData.accel[1];
  accel_sum_for_check_launch[2] += accelAndGyroData.accel[2];
  if (accel_data_count_for_check_launch == NUMBER_OF_ACCEL_DATA_FOR_LAUNCH) {
    // 離床判断
    double accel_av_square_sum = pow(accel_sum_for_check_launch[0] / NUMBER_OF_ACCEL_DATA_FOR_LAUNCH, 2) +
                                 pow(accel_sum_for_check_launch[1] / NUMBER_OF_ACCEL_DATA_FOR_LAUNCH, 2) +
                                 pow(accel_sum_for_check_launch[2] / NUMBER_OF_ACCEL_DATA_FOR_LAUNCH, 2);
    Serial.printf("accel_av_square_sum: %0.4f\n", accel_av_square_sum);
    if (accel_av_square_sum >= ACCEL_SQUARE_SUM_THRESHOLD) {
      isLaunched = true;
      launch_time = millis();
    }
    // データをリセット
    accel_sum_for_check_launch[0] = 0;
    accel_sum_for_check_launch[1] = 0;
    accel_sum_for_check_launch[2] = 0;
    accel_data_count_for_check_launch = 0;
  }
  accel_data_count_for_check_launch++;
  return isLaunched;
}

bool ConditionChecker::checkLaunchByPressure(PressureData pressureData) {
  pressure_sum_for_check_launch += pressureData.pressure;
  if (pressure_data_count_for_check_launch == NUMBER_OF_PRESSURE_DATA_FOR_LAUNCH) {
    uint32_t pressure_av = pressure_sum_for_check_launch / NUMBER_OF_PRESSURE_DATA_FOR_LAUNCH;
    // Serial.printf("pressure_av: %d\n", pressure_av);
    int32_t pressure_diff = last_pressure_av_for_check_launch - pressure_av;
    Serial.printf("pressure_diff: %d\n", pressure_diff);
    if (pressure_diff > 0 && pressure_diff > PRESSURE_AV_THRESHOLD_FOR_LAUNCH) {
      pressure_decrease_count_for_check_launch++;
    } else {
      pressure_decrease_count_for_check_launch = 0;
    }
    last_pressure_av_for_check_launch = pressure_av;
    // Serial.printf("pressure_decrease_count_for_check_launch: %d\n", pressure_decrease_count_for_check_launch);
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
  pressure_sum_for_check_apogee += pressureData.pressure;
  if (pressure_data_count_for_check_apogee == NUMBER_OF_PRESSURE_DATA_FOR_APOGEE) {
    uint32_t pressure_av = pressure_sum_for_check_apogee / NUMBER_OF_PRESSURE_DATA_FOR_APOGEE;
    // Serial.printf("pressure_av: %d\n", pressure_av);
    int32_t pressure_diff = pressure_av - last_pressure_av_for_check_apogee;
    // Serial.printf("pressure_diff: %d\n", pressure_diff);
    if (pressure_diff > 0 && pressure_diff > PRESSURE_AV_DIFFERENCE_THRESHOLD_FOR_APOGEE) {
      pressure_increase_count_for_check_apogee++;
    } else {
      pressure_increase_count_for_check_apogee = 0;
    }
    last_pressure_av_for_check_apogee = pressure_av;
    // Serial.printf("pressure_increase_count_for_check_apogee: %d\n", pressure_increase_count_for_check_apogee);
    if (pressure_increase_count_for_check_apogee >= PRESSURE_INCREASE_COUNT_THRESHOLD_FOR_APOGEE) {
      hasReachedApogee = true;
    }
    pressure_sum_for_check_apogee = 0;
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
    Serial.println("reached apogee by timer");
  }
  return hasReachedApogee;
}

bool ConditionChecker::getIsLaunched() { return isLaunched; }

bool ConditionChecker::getHasReachedApogee() { return hasReachedApogee; }