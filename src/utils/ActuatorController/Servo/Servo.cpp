#include "utils/ActuatorController/Servo/Servo.h"
#include "driver/ledc.h"
#include "esp32-hal-ledc.h"

void Servo::begin(gpio_num_t gpio) {
  ledc = LEDC_CHANNEL_0;
  this->gpio = gpio;
  frequency = 50;
  bit = 10;
  min = ((0.5 / 20) * (1 << bit)) + 0.5; // (0.5ms/20ms)*1024 + 0.5(round)
  max = ((2.4 / 20) * (1 << bit)) + 0.5; // (2.4ms/20ms)*1024 + 0.5(round)

  ledcAttachChannel(gpio, frequency, bit, ledc);
}

void Servo::write(uint8_t degree) {
  if (180 < degree) {
    degree = 90;
  }
  ledcWrite(ledc, min + ((max - min) * degree / 180));
}
