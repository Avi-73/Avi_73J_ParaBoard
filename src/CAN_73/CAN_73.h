/**
 * @file CAN_73.h
 * @brief CAN通信を行うクラス
 * @details
 * data[0]に送信元のボードID、data[1]に送信データ(char)を格納する
 */

#pragma once
#ifndef CAN_73_H
#define CAN_73_H

#include "CAN_config.h"
#include <driver/gpio.h>
#include <driver/twai.h>

class CAN_73 {
public:
  /**
   * begin
   * @param rx_pin: 受信ピン
   * @param tx_pin: 送信ピン
   * @param own_board_id: 自分の基板のボードID
   */
  void begin(gpio_num_t rx_pin, gpio_num_t tx_pin, BoardId own_board_id);

  /**
   * available
   * @return 受信データがあるかどうか
   */
  bool available();

  /**
   * read
   * @return 受信データ
   */
  can_message_t readWithDetails();

  /**
   * read
   * @return 受信データ
   */
  char read();

  /**
   * write
   * @param id: 送信先のボードID
   * @param data: 送信データ
   */
  void write(BoardId id, char data);

private:
  BoardId own_board_id;
  twai_general_config_t g_config;
  twai_timing_config_t t_config;
  twai_filter_config_t f_config;
  twai_message_t rx_msg;
  twai_message_t tx_msg;
};

#endif