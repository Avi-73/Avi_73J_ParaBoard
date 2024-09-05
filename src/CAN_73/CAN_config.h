#pragma once
#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

enum BoardId {
  COMMUNICATION = 0x004,
  PARA = 0x001,
  BATTERY = 0x005,
  REAL_TIME_VIDEO = 0x002,
  GIMBAL = 0x003,
};

/**
 * can_message_t
 * @brief CAN通信で送受信するデータの構造体
 * @details
 * source_board_id: 送信元のボードID
 * data: 送信データ
 */
struct can_message_t {
  // 送信元のボードID
  BoardId source_board_id;
  // 送信データ
  char data;
};

#endif