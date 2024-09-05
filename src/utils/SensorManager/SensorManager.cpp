#include "utils/SensorManager/SensorManager.h"
#include "utils/Config.h"

void SensorManager::begin() {
  spi.begin(VSPI, SPICLOCK, SPIMISO, SPIMOSI, SPI_FREQUENCY);
  Serial.println("SPI begin");
  lps.begin(&spi, LPSCS, SPI_FREQUENCY);
  Serial.println("LPS begin");
  icm.begin(&spi, ICMCS, SPI_FREQUENCY);
  Serial.println("ICM begin");
}

AccelAndGyroData SensorManager::getAccelAndGyroData() {
  int16_t ICM_data[6];
  icm.Get(ICM_data);
  AccelAndGyroData accelAndGyroData;
  accelAndGyroData.accel[0] = ICM_data[0];
  accelAndGyroData.accel[1] = ICM_data[1];
  accelAndGyroData.accel[2] = ICM_data[2];
  accelAndGyroData.gyro[0] = ICM_data[3];
  accelAndGyroData.gyro[1] = ICM_data[4];
  accelAndGyroData.gyro[2] = ICM_data[5];
  return AccelAndGyroData();
};

PressureData SensorManager::getPressureData() {
  uint8_t LPS25_data[3];
  lps.Get(LPS25_data);
  PressureData pressureData;
  pressureData.pressure = (LPS25_data[0] + LPS25_data[1] * 256 + LPS25_data[2] * 65536);
  return pressureData;
  return PressureData();
};
