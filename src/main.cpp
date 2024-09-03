#include <Arduino.h>
#include "utils/SensorManager/SensorManager.h"
#include "utils/FlightController/FlightController.h"
#include "utils/CommunicationManager/CommunicationManager.h"
#include "utils/DataLogger/DataLogger.h"
#include "utils/ActuatorController/ActuatorController.h"

SensorManager sensorManager;
FlightController flightController;
CommunicationManager commManager;
DataLogger dataLogger;
ActuatorController actuatorController;

void setup() {
  sensorManager.begin();
  flightController.begin();
  commManager.begin();
  dataLogger.begin();
  actuatorController.begin();
  
  // タイマー割り込みの設定
  // (タイマー設定コードをここに移動)
}

void loop() {
  commManager.processCommands();
  
  if (sensorManager.dataReady()) {
    SensorData data = sensorManager.getData();
    flightController.update(data);
    dataLogger.logData(data);
    
    if (flightController.shouldDeployParachute()) {
      actuatorController.deployParachute();
    }
  }
  
  actuatorController.updateLED(flightController.getFlightState());
}

// タイマー割り込み関数
void IRAM_ATTR onTimer() {
  sensorManager.triggerRead();
}