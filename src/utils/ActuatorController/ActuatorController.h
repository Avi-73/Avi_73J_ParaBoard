#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include "utils/Config.h"
#include "utils/DataLogger/DataLogger.h"
#include <ESP32Servo.h>

class ActuatorController {
public:
  void begin();
  void openServo();
  void closeServo();
  void increaseServoOpenAngle();
  void decreaseServoOpenAngle();
  void increaseServoCloseAngle();
  void decreaseServoCloseAngle();

private:
  Servo servo;
  uint8_t servo_open_angle;
  uint8_t servo_close_angle;
};

#endif