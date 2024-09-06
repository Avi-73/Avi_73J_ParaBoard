#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// SPI設定
const int SPICLOCK = 13;
const int SPIMISO = 15;
const int SPIMOSI = 2;
const int LPSCS = 12;
const int ICMCS = 4;
const long SPI_FREQUENCY = 6000000;

// CAN設定
const int CAN_RX = 5;
const int CAN_TX = 17;

// シリアル通信設定
const long SERIAL_BAUD_RATE = 115200;

// サーボモーター設定
const int SERVO_PIN = 26;

// LED設定
const int LED_PIN = 22;

// microSDカード設定
const int SD_CS = 16;
const int SD_MOSI = 23;
const int SD_MISO = 19;
const int SD_SCK = 18;

// 加速度による離床検知の設定
/** 判定に必要な加速度データの数 */
const int16_t NUMBER_OF_ACCEL_DATA_FOR_LAUNCH = 20;
/** 判定に必要な加速度の二乗和の閾値(G^2) */
const int16_t ACCEL_SQUARE_SUM_THRESHOLD = 4;
// const int16_t ACCEL_SQUARE_SUM_THRESHOLD = 2;

// 気圧による離床検知の設定
/** 判定に必要な気圧データの数 */
const uint32_t NUMBER_OF_PRESSURE_DATA_FOR_LAUNCH = 5;
/** 判定に必要な気圧の和の閾値(Pa) */
const uint32_t PRESSURE_AV_THRESHOLD_FOR_LAUNCH = 10;
/** 気圧が減少した回数の閾値 */
const int8_t PRESSURE_DECREASE_COUNT_THRESHOLD_FOR_LAUNCH = 5;

// 気圧による頂点検知の設定
/** 判定に必要な気圧データの数 */
const int8_t NUMBER_OF_PRESSURE_DATA_FOR_APOGEE = 5;
/** 判定に必要な気圧の差の閾値(Pa) */
const int16_t PRESSURE_AV_DIFFERENCE_THRESHOLD_FOR_APOGEE = 0;
/** 気圧が増加した回数の閾値 */
const int8_t PRESSURE_INCREASE_COUNT_THRESHOLD_FOR_APOGEE = 5;

// タイマーによる頂点検知の設定
/** 離床検知から何秒立ったら頂点とするか(ms) */
const uint32_t TIME_THRESHOLD_FOR_APOGEE_FROM_LAUNCH = 9000;
#endif