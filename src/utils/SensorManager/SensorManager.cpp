#include "utils/SensorManager/SensorManager.h"
#include "utils/Config.h"

void SensorManager::begin() {
    spi.begin(VSPI, SPICLOCK, SPIMISO, SPIMOSI);
    lps.begin(&spi, LPSCS, SPI_FREQUENCY);
    icm.begin(&spi, ICMCS, SPI_FREQUENCY);
    _dataReady = false;
}

void SensorManager::triggerRead() {
    int16_t ICM_data[6];
    uint8_t LPS25_data[3];
    
    icm.Get(ICM_data);
    lps.Get(LPS25_data);
    
    for (int i = 0; i < 3; i++) {
        latestData.accel[i] = ICM_data[i];
        latestData.gyro[i] = ICM_data[i+3];
    }
    
    latestData.pressure = (LPS25_data[0] + LPS25_data[1] * 256 + LPS25_data[2] * 65536);
    
    _dataReady = true;
}

bool SensorManager::dataReady() {
    return _dataReady;
}

SensorData SensorManager::getData() {
    _dataReady = false;
    return latestData;
}