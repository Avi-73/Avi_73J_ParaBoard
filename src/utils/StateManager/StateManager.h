#pragma once

enum class State {
  IDLE,
  FLIGHT,
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