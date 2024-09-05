#include "StateManager.h"

void StateManager::begin() {
  state = State::IDLE;
}

void StateManager::changeState(State newState) {
  state = newState;
}

State StateManager::getState() {
  return state;
}
