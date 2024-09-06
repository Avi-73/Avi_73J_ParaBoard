#include <Arduino.h>

#include "CAN_73/CAN_73.h"
#include "esp_task_wdt.h"
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
CAN_73 can;

// タスクハンド
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
  State state;

  for (;;) {
    if (can.available()) {
      cmd = can.read();
      // if (Serial.available()) {
      //   cmd = Serial.read();

      // 　コマンドによる処理の分岐
      switch (cmd) {
      case Command::LOG:
        // -> WAITING_LAUNCH MODE
        if (stateManager.getState() != State::FLIGHT) {
          vTaskResume(conditionsCheckTaskHandle);
          vTaskResume(logTaskHandle);
          timer.start();
          stateManager.changeState(State::FLIGHT);
        }
        Serial.println(Command::LOG);
        can.write(BoardId::COMMUNICATION, cmd);

        break;
      case Command::END_LOG:
        // -> IDLE MODE
        if (stateManager.getState() == State::FLIGHT) {
          timer.stop();
          vTaskSuspend(conditionsCheckTaskHandle);
          vTaskSuspend(logTaskHandle);
          stateManager.changeState(State::IDLE);
          Serial.println(Command::END_LOG);
          can.write(BoardId::COMMUNICATION, cmd);
          conditionChecker.begin();
        }
        if (stateManager.getState() == State::DEBUG) {
          stateManager.changeState(State::IDLE);
          Serial.println(Command::END_LOG);
          can.write(BoardId::COMMUNICATION, cmd);
        }
        break;
      case Command::DEBUG:
        // -> DEBUG MODE
        state = stateManager.getState();
        if (state != State::DEBUG) {
          if (state == State::FLIGHT) {
            timer.stop();
          }
          vTaskResume(conditionsCheckTaskHandle);
          vTaskResume(logTaskHandle);
          stateManager.changeState(State::DEBUG);
          Serial.println(Command::DEBUG);
          can.write(BoardId::COMMUNICATION, cmd);
        }
        break;
      case Command::OPEN_PARACHUTE:
        actuatorController.openServo();
        can.write(BoardId::COMMUNICATION, cmd);
        Serial.println(Command::OPEN_PARACHUTE);
        break;
      case Command::CLOSE_PARACHUTE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.closeServo();
          can.write(BoardId::COMMUNICATION, cmd);
          Serial.println(Command::CLOSE_PARACHUTE);
        }
        break;
      case Command::INCREASE_OPEN_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.increaseServoOpenAngle();
          Serial.println(Command::INCREASE_OPEN_SERVO_ANGLE);
          can.write(BoardId::COMMUNICATION, cmd);
        }
        break;
      case Command::DECREASE_OPEN_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.decreaseServoOpenAngle();
          Serial.println(Command::DECREASE_OPEN_SERVO_ANGLE);
          can.write(BoardId::COMMUNICATION, cmd);
        }
        break;
      case Command::INCREASE_CLOSE_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.increaseServoCloseAngle();
          Serial.println(Command::INCREASE_CLOSE_SERVO_ANGLE);
          can.write(BoardId::COMMUNICATION, cmd);
        }
        break;
      case Command::DECREASE_CLOSE_SERVO_ANGLE:
        if (stateManager.getState() == State::DEBUG) {
          actuatorController.decreaseServoCloseAngle();
          Serial.println(Command::DECREASE_CLOSE_SERVO_ANGLE);
          can.write(BoardId::COMMUNICATION, cmd);
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
  bool has_opened_servo = false;
  bool has_send_cmd_launch = false;
  bool has_send_cmd_apogee = false;

  while (1) {
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
        // Serial.println("checking launch");
        if (!has_send_cmd_launch) {
          Serial.println("launch detected");
          can.write(BoardId::COMMUNICATION, Command::LAUNCH_DETECTED);
          has_send_cmd_launch = true;
        }
      }
    }

    // 頂点検知
    if (conditionChecker.getIsLaunched() && !conditionChecker.getHasReachedApogee()) {
      conditionChecker.checkApogeeByTimer();
      if (count % 40 == 0) {
        conditionChecker.checkApogeeByPressure(sensorManager.getPressureData());
        // Serial.println("launch detected, checking apogee");
      }
    }

    // 頂点検知した際の処理
    if (conditionChecker.getHasReachedApogee() && !has_opened_servo) {
      actuatorController.openServo();
      has_opened_servo = true;
      if (!has_send_cmd_apogee) {
        Serial.println("reached apogee");
        can.write(BoardId::COMMUNICATION, Command::APOGEE_DETECTED);
        has_send_cmd_apogee = true;
      }
    }

    // ログタスク通知
    logData.timestamp = timestamp;
    logData.accelAndGyroData = accelAndGyroData;
    logData.pressureData = pressureData;
    ret = xQueueSendFromISR(dataQueue, &logData, 0);
    if (ret != pdPASS) {
      // Serial.println("Queue overwrite failed");
    } else {
      // Serial.println("Queue send success");
    }
    BaseType_t Taskwoken = pdFALSE;
    vTaskNotifyGiveFromISR(logTaskHandle, &Taskwoken);
    if (Taskwoken == pdTRUE) {
      portYIELD_FROM_ISR();
    }
    vTaskDelay(1);
  }
}

/** ログタスク */
void LogTask(void *pvParameters) {
  LogData logData;
  BaseType_t ret;

  while (1) {
    // タスクが再開されるのを待つ
    xTaskNotifyWait(pdTRUE, pdTRUE, NULL, portMAX_DELAY);

    // queueからデータを取り出す
    ret = xQueueReceive(dataQueue, &logData, pdMS_TO_TICKS(10)); // タイムアウトを短くする
    if (ret == pdPASS) {
      dataLogger.logData(logData);
    }
    vTaskDelay(1);
  }
}

static bool IRAM_ATTR onTimer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  BaseType_t Taskwoken = pdFALSE;
  vTaskNotifyGiveFromISR(conditionsCheckTaskHandle, &Taskwoken);
  return (Taskwoken == pdTRUE);
}

void setup() {
  Serial.begin(115200);
  can.begin((gpio_num_t)CAN_RX, (gpio_num_t)CAN_TX, BoardId::PARA);

  dataLogger.begin();
  stateManager.begin();
  actuatorController.begin();
  sensorManager.begin();
  conditionChecker.begin();

  dataQueue = xQueueCreate(100, sizeof(LogData));

  if (!timer.init(40000000, 40000)) {
    Serial.println("Timer init failed");
  }
  timer.registerCallback(onTimer);

  xTaskCreatePinnedToCore(StateControlTask, "State Control Task", 2048, NULL, 1, &stateControlTaskHandle, 1);
  xTaskCreatePinnedToCore(ConditionsCheckTask, "Conditions Check Task", 8192, NULL, 1, &conditionsCheckTaskHandle, 1);
  xTaskCreatePinnedToCore(LogTask, "Log Task", 4096, NULL, 2, &logTaskHandle, 0);

  vTaskSuspend(conditionsCheckTaskHandle);
  vTaskSuspend(logTaskHandle);
}

void loop() { vTaskDelay(10000); }