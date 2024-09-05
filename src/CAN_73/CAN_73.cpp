#include "CAN_73.h"
#include <Arduino.h>

void CAN_73::begin(gpio_num_t rx_pin, gpio_num_t tx_pin, BoardId own_board_id) {
  g_config = TWAI_GENERAL_CONFIG_DEFAULT(rx_pin, tx_pin, TWAI_MODE_NORMAL);
  t_config = TWAI_TIMING_CONFIG_100KBITS();
  f_config = {.acceptance_code = (uint32_t)own_board_id << 21,
              .acceptance_mask = ((uint32_t)(own_board_id ^ 0x12F) << 21) | 0x1fffff,
              .single_filter = true};
  this->own_board_id = own_board_id;

  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("CAN Driver installed");
  } else {
    Serial.println("[ERROR] Failed to install CAN Driver");
    return;
  }

  if (twai_start() == ESP_OK) {
    Serial.println("CAN Driver started");
  } else {
    Serial.println("[ERROR] Failed to start CAN Driver");
    return;
  }
}

bool CAN_73::available() { return twai_receive(&rx_msg, 0) == ESP_OK; }

can_message_t CAN_73::readWithDetails() {
  can_message_t message;
  if (rx_msg.data_length_code > 0) {
    message.source_board_id = (BoardId)rx_msg.data[0];
    message.data = rx_msg.data[1];
    return message;
  }
  return can_message_t();
}

char CAN_73::read() {
  if (rx_msg.data_length_code > 0) {
    return rx_msg.data[1];
  }
  return 0;
}

void CAN_73::write(BoardId id, char data) {
  tx_msg.identifier = id;
  tx_msg.extd = 0;
  tx_msg.data_length_code = 2;
  tx_msg.data[0] = own_board_id;
  tx_msg.data[1] = data;

  esp_err_t ret = twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));
  if (ret != ESP_OK) {
    Serial.printf("[ERROR] Failed to send message: %s\n", esp_err_to_name(ret));
  }
}
