#include "utils/SensorManager/SensorManager.h"
#include "utils/Config.h"

void SensorManager::begin() {
  spi.begin(HSPI, SPICLOCK, SPIMISO, SPIMOSI, SPI_FREQUENCY);
  lps.begin(&spi, LPSCS, SPI_FREQUENCY);
  icm.begin(&spi, ICMCS, SPI_FREQUENCY);
}

AccelAndGyroData SensorManager::getAccelAndGyroData() {
  int16_t ICM_data[6];
  icm.Get(ICM_data);
  AccelAndGyroData accelAndGyroData;
  accelAndGyroData.accel[0] = ICM_data[0] / 2048.0;
  accelAndGyroData.accel[1] = ICM_data[1] / 2048.0;
  accelAndGyroData.accel[2] = ICM_data[2] / 2048.0;
  accelAndGyroData.gyro[0] = ICM_data[3] / 2048.0;
  accelAndGyroData.gyro[1] = ICM_data[4] / 2048.0;
  accelAndGyroData.gyro[2] = ICM_data[5] / 2048.0;
  // double型でそれぞれを2048で割ったものの2乗の和を表示する
  //   Serial.printf("加速度の2乗和: %.2f\n", pow(accelAndGyroData.accel[0], 2) + pow(accelAndGyroData.accel[1], 2) +
  //                                              pow(accelAndGyroData.accel[2], 2));
  return accelAndGyroData;
};

PressureData SensorManager::getPressureData() {
  uint8_t LPS25_data[3];
  lps.Get(LPS25_data);
  PressureData pressureData;
  pressureData.pressure = (LPS25_data[0] + LPS25_data[1] * 256 + LPS25_data[2] * 65536) * 100 / 4096;
  return pressureData;
};
