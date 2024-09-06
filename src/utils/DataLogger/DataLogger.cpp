#include "utils/DataLogger/DataLogger.h"
#include "utils/Config.h"
#include "utils/SensorData.h"

void DataLogger::begin() {
  if (!sd.begin(SD_CS, SD_SCK_MHZ(25))) {
    Serial.println("[ERR] Cannot Initialize SD Card.");
    can_open_file = false;
    return;
  }
  Serial.println("SD Card Initialized.");

  File32 root = sd.open("/");
  int8_t count = 0;
  File32 entry;
  while (entry.openNext(&root, O_RDONLY)) {
    count++;
    entry.close();
  }
  root.close();

  snprintf(filename, sizeof(filename), "dat%d.csv", count);
  if (!file.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
    Serial.println("[ERR] Cannot Create File.");
    can_open_file = false;
  } else {
    can_open_file = true;
    file.println("timestamp,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,pressure");
    file.sync();
  }
}

void DataLogger::logData(const LogData &data) {
  if (!can_open_file) {
    return;
  }
  file.printf("%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d\n", data.timestamp, data.accelAndGyroData.accel[0],
              data.accelAndGyroData.accel[1], data.accelAndGyroData.accel[2], data.accelAndGyroData.gyro[0],
              data.accelAndGyroData.gyro[1], data.accelAndGyroData.gyro[2], data.pressureData.pressure);

  if (!file.sync()) {
    // Serial.println("Sync error");
  }
}

void DataLogger::end() {
  if (!can_open_file) {
    return;
  }
  file.close();
}
