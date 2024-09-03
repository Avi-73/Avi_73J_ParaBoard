#include "utils/CommunicationManager/CommunicationManager.h"
#include "utils/Config.h"

void CommunicationManager::begin() {
    CAN.setPins(CAN_RX, CAN_TX);
    CAN.begin(CAN_FREQUENCY);
}

void CommunicationManager::processCommands() {
    if (CAN.available()) {
        char cmd = (char)CAN.read();
        handleCANCommand(cmd);
    }
}

void CommunicationManager::handleCANCommand(char cmd) {
    // CANコマンドの処理
    switch (cmd) {
        case 'a':
            // サーボ制御などの処理
            break;
        // 他のコマンド処理
    }
}