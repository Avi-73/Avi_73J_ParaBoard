#pragma once

enum class State {
  IDLE,
  WAITING_LAUNCH,
  WAITING_APOGEE,
  DEBUG,
};

class StateManager {
public:
  void begin();
  void changeState(State state);
  State getState();

private:
  State state;
};