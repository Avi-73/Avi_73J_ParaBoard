#ifndef CONFIG_H
#define CONFIG_H

// SPI設定
const int SPICLOCK = 33;
const int SPIMISO = 25;
const int SPIMOSI = 26;
const int FLASHCS = 27;
const int LPSCS = 13;
const int ICMCS = 15;
const long SPI_FREQUENCY = 6000000;

// CAN設定
const int CAN_RX = 17;
const int CAN_TX = 18;
const long CAN_FREQUENCY = 100E3;

// シリアル通信設定
const long SERIAL_BAUD_RATE = 115200;

// サーボモーター設定
const int SERVO_PIN = 14;
const int SERVO_INITIAL_POSITION = 90;
const int SERVO_DEPLOY_POSITION = 0;

// LED設定
const int LED_PIN = 22;

// microSDカード設定
const int SD_CS = 5;
const int SD_MOSI = 23;
const int SD_MISO = 19;
const int SD_SCK = 18;

#endif