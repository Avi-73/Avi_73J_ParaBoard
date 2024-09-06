#include "utils/ActuatorController/ActuatorController.h"
#include "utils/Config.h"

#include <Arduino.h>

void ActuatorController::begin() {
  pinMode(LED_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo_open_angle = 120;
  servo_close_angle = 80;
  servo.write(servo_close_angle);
}

void ActuatorController::openServo() { servo.write(servo_open_angle); }

void ActuatorController::closeServo() { servo.write(servo_close_angle); }

void ActuatorController::increaseServoOpenAngle() {
  if (servo_open_angle + 1 > 180) {
    servo_open_angle = 180;
  } else {
    servo_open_angle += 1;
  }
  servo.write(servo_open_angle);
}

void ActuatorController::decreaseServoOpenAngle() {
  if (servo_open_angle - 1 < 0) {
    servo_open_angle = 0;
  } else {
    servo_open_angle -= 1;
  }
  servo.write(servo_open_angle);
}

void ActuatorController::increaseServoCloseAngle() {
  if (servo_close_angle + 1 > 180) {
    servo_close_angle = 180;
  } else {
    servo_close_angle += 1;
  }
  servo.write(servo_close_angle);
}

void ActuatorController::decreaseServoCloseAngle() {
  if (servo_close_angle - 1 < 0) {
    servo_close_angle = 0;
  } else {
    servo_close_angle -= 1;
  }
  servo.write(servo_close_angle);
}