#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include <ESP32Servo.h>
#include "utils/FlightController/FlightController.h"

class ActuatorController {
public:
  void begin();
  void deployParachute();
  void updateLED(FlightState state);
  void setCameraState(bool on);

private:
  Servo servo;
  const int LED_PIN = 22;
};

#endif