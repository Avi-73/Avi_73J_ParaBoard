#include "utils/ActuatorController/ActuatorController.h"
#include "utils/Config.h"

void ActuatorController::begin() {
    pinMode(LED_PIN, OUTPUT);
    servo.attach(SERVO_PIN);
    servo.write(SERVO_INITIAL_POSITION);
}

void ActuatorController::deployParachute() {
    servo.write(SERVO_DEPLOY_POSITION);
}

void ActuatorController::updateLED(FlightState state) {
    switch (state) {
        case STANDBY:
            digitalWrite(LED_PIN, HIGH);
            break;
        case LIFTOFF:
            digitalWrite(LED_PIN, LOW);
            break;
        case FREEFALL:
            // LED点滅など
            break;
        case PARACHUTE_DEPLOYED:
            digitalWrite(LED_PIN, HIGH);
            break;
    }
}