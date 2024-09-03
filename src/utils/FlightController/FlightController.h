#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "utils/SensorManager/SensorManager.h"

enum FlightState {
  DEBUG,
  SLEEP,          // 待機状態 
  STANDBY,        // 発射前の待機状態
  LIFTOFF,        // 打ち上げ直後の上昇状態
  COAST,          // エンジン燃焼終了後の慣性飛行状態
  APOGEE,         // 頂点（最高高度）に達した状態
  FREEFALL,       // パラシュート展開前の自由落下状態
  PARACHUTE_DEPLOYED, // パラシュート展開後の降下状態
};

class FlightController {
public:
  void begin();
  void update(const SensorData& data);
  bool shouldDeployParachute();
  FlightState getFlightState();

private:
  FlightState currentState;
  bool detectLiftoff(const SensorData& data);
  bool detectApogee(const SensorData& data);
  bool detectLanding(const SensorData& data);
  bool detectDeploymentCondition();
};

#endif