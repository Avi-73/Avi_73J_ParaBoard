#include "utils/DataLogger/DataLogger.h"
#include "utils/Config.h"

void DataLogger::begin() {
    // flash.begin(&spi, FLASHCS, SPI_FREQUENCY);
    // currentAddress = 0;
}

void DataLogger::logData(const SensorData& data) {
    // uint8_t buffer[LOG_ENTRY_SIZE];
    
    // // データをバッファに格納
    // memcpy(buffer, &data, sizeof(SensorData));
    
    // // タイムスタンプなどの追加情報を格納
    // uint32_t timestamp = millis();
    // memcpy(buffer + sizeof(SensorData), &timestamp, sizeof(uint32_t));
    
    // // フラッシュメモリに書き込み
    // flash.write(currentAddress, buffer, LOG_ENTRY_SIZE);
    // currentAddress += LOG_ENTRY_SIZE;
}

void DataLogger::retrieveData() {
    // for (uint32_t addr = 0; addr < currentAddress; addr += 32) {
    //     uint8_t buffer[32];
    //     flash.read(addr, buffer, 32);
        
    //     // データの解析と表示
    //     SensorData data;
    //     memcpy(&data, buffer, sizeof(SensorData));
        
    //     uint32_t timestamp;
    //     memcpy(&timestamp, buffer + sizeof(SensorData), sizeof(uint32_t));
        
    //     // データの表示またはシリアル送信
    //     Serial.print("Time: ");
    //     Serial.print(timestamp);
    //     Serial.print(", Accel: ");
    //     for (int i = 0; i < 3; i++) {
    //         Serial.print(data.accel[i]);
    //         Serial.print(" ");
    //     }
    //     Serial.print(", Gyro: ");
    //     for (int i = 0; i < 3; i++) {
    //         Serial.print(data.gyro[i]);
    //         Serial.print(" ");
    //     }
    //     Serial.print(", Pressure: ");
    //     Serial.println(data.pressure);
    // }
}