#include <Arduino.h>

#include "utils/ActuatorController/ActuatorController.h"
#include "utils/Command.h"
#include "utils/ConditionChecker/ConditionChecker.h"
#include "utils/Config.h"
#include "utils/DataLogger/DataLogger.h"
#include "utils/GPTimer/GPTimer.h"
#include "utils/SensorData.h"
#include "utils/SensorManager/SensorManager.h"
#include "utils/StateManager/StateManager.h"

// マネージャークラス
SensorManager sensorManager;
DataLogger dataLogger;
StateManager stateManager;
ActuatorController actuatorController;
ConditionChecker conditionChecker;

// タスクハンドル
TaskHandle_t stateControlTaskHandle = NULL;
TaskHandle_t conditionsCheckTaskHandle = NULL;
TaskHandle_t logTaskHandle = NULL;
QueueHandle_t dataQueue;

// タイマー
GPTimer timer; // 1kHzで割り込み

// タスク
/** 状態制御タスク */
void StateControlTask(void *pvParameters) {
  char cmd;

  for (;;) {
    if (Serial.available()) { // TODO: CANに変更
      cmd = Serial.read();

      // 　コマンドによる処理の分岐
      switch (cmd) {
      case Command::LOG:
        // -> WAITING_LAUNCH MODE
        if (stateManager.getState() != State::WAITING_LAUNCH) {
          vTaskResume(conditionsCheckTaskHandle);
          vTaskResume(logTaskHandle);
          timer.start();
        }
        break;
      case Command::END_LOG:
        // -> IDLE MODE
        if (stateManager.getState() == State::DEBUG) {
          vTaskSuspend(conditionsCheckTaskHandle);
          vTaskSuspend(logTaskHandle);
          timer.stop();
          stateManager.changeState(State::IDLE);
        }
        break;
      case Command::DEBUG:
        // -> DEBUG MODE
        if (stateManager.getState() != State::DEBUG) {
          timer.stop();
          vTaskResume(conditionsCheckTaskHandle);
          vTaskResume(logTaskHandle);
          stateManager.changeState(State::DEBUG);
        }
        break;
      case Command::OPEN_PARACHUTE:
        actuatorController.openServo();
        break;
      case Command::CLOSE_PARACHUTE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.closeServo();
        }
        break;
      case Command::INCREASE_OPEN_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.increaseServoOpenAngle();
        }
        break;
      case Command::DECREASE_OPEN_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.decreaseServoOpenAngle();
        }
        break;
      case Command::INCREASE_CLOSE_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.increaseServoCloseAngle();
        }
        break;
      case Command::DECREASE_CLOSE_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.decreaseServoCloseAngle();
        }
        break;
      }
    }
  }
}

/** 条件チェックタスク */
void ConditionsCheckTask(void *pvParameters) {
  uint32_t count = 0;
  BaseType_t ret;
  AccelAndGyroData accelAndGyroData;
  PressureData pressureData;
  LogData logData;
  uint32_t timestamp;

  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    accelAndGyroData = sensorManager.getAccelAndGyroData();

    if (count % 40 == 0) {
      pressureData = sensorManager.getPressureData();
    }
    timestamp = millis();
    // 離床検知
    if (!conditionChecker.getIsLaunched()) {
      conditionChecker.checkLaunchByAccel(sensorManager.getAccelAndGyroData());
      if (count % 40 == 0) {
        conditionChecker.checkLaunchByPressure(sensorManager.getPressureData());
      }
    }

    // 頂点検知
    if (conditionChecker.getIsLaunched()) {
      conditionChecker.checkApogeeByTimer();
      if (count % 40 == 0) {
        conditionChecker.checkApogeeByPressure(sensorManager.getPressureData());
      }
    }

    // 頂点検知した際の処理
    if (conditionChecker.getHasReachedApogee()) {
      actuatorController.openServo();
    }

    // ログタスク通知
    logData.timestamp = timestamp;
    logData.accelAndGyroData = accelAndGyroData;
    logData.pressureData = pressureData;
    ret = xQueueOverwrite(dataQueue, &logData);
    BaseType_t Taskwoken = pdFALSE;
    vTaskNotifyGiveFromISR(logTaskHandle, &Taskwoken);
    if (Taskwoken == pdTRUE) {
      portYIELD_FROM_ISR();
    }
  }
}

/** ログタスク */
void LogTask(void *pvParameters) {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
}

static bool IRAM_ATTR onTimer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  BaseType_t Taskwoken = pdFALSE;
  vTaskNotifyGiveFromISR(conditionsCheckTaskHandle, &Taskwoken);
  return (Taskwoken == pdTRUE);
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  dataLogger.begin();
  stateManager.begin();
  actuatorController.begin();
  sensorManager.begin();
  conditionChecker.begin();

  dataQueue = xQueueCreate(10, sizeof(LogData));
  Serial.println("dataQueue");

  if (!timer.init(40000000, 40000)) {
    Serial.println("Timer init failed");
  }
  timer.registerCallback(onTimer);

  xTaskCreatePinnedToCore(StateControlTask, "State Control Task", 2048, NULL, 1, &stateControlTaskHandle, 1);
  xTaskCreatePinnedToCore(ConditionsCheckTask, "Conditions Check Task", 2048, NULL, 1, &conditionsCheckTaskHandle, 1);
  xTaskCreatePinnedToCore(LogTask, "Log Task", 2048, NULL, 3, &logTaskHandle, 0);

  vTaskSuspend(conditionsCheckTaskHandle);
  vTaskSuspend(logTaskHandle);
}

void loop() { vTaskDelay(10000); }