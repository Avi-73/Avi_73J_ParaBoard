#include "utils/ActuatorController/ActuatorController.h"
#include "utils/Config.h"

#include <Arduino.h>

void ActuatorController::begin() {
  pinMode(LED_PIN, OUTPUT);
  Serial.printf("SERVO_PIN: %d\n", SERVO_PIN);
  servo.begin((gpio_num_t)SERVO_PIN);
  servo.write(servo_close_angle);
}

void ActuatorController::openServo() { servo.write(servo_open_angle); }

void ActuatorController::closeServo() { servo.write(servo_close_angle); }

void ActuatorController::increaseServoOpenAngle() {
  if (servo_open_angle + 10 > 180) {
    servo_open_angle = 180;
  } else {
    servo_open_angle += 10;
  }
  servo.write(servo_open_angle);
}

void ActuatorController::decreaseServoOpenAngle() {
  if (servo_open_angle - 10 < 0) {
    servo_open_angle = 0;
  } else {
    servo_open_angle -= 10;
  }
  servo.write(servo_open_angle);
}

void ActuatorController::increaseServoCloseAngle() {
  if (servo_close_angle + 10 > 180) {
    servo_close_angle = 180;
  } else {
    servo_close_angle += 10;
  }
  servo.write(servo_close_angle);
}

void ActuatorController::decreaseServoCloseAngle() {
  if (servo_close_angle - 10 < 0) {
    servo_close_angle = 0;
  } else {
    servo_close_angle -= 10;
  }
  servo.write(servo_close_angle);
}