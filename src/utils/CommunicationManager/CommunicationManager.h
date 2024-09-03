#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "utils/CAN/CAN.h"

class CommunicationManager {
public:
  void begin();
  void processCommands();

private:
  CAN_CREATE CAN;
  void handleCANCommand(char cmd);
};

#endif