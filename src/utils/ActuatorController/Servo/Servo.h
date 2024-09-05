#pragma once

#include "driver/ledc.h"
#include <Arduino.h>

class Servo {
public:
  void begin(gpio_num_t gpio);
  void write(uint8_t degree);

private:
  ledc_channel_t ledc;
  gpio_num_t gpio;
  uint16_t frequency;
  uint8_t bit;
  uint16_t min;
  uint16_t max;
};