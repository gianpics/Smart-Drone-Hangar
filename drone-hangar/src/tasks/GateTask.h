#ifndef __GATE_TASK__
#define __GATE_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Gate.h"
#include <Arduino.h>

class GateTask: public Task {

public:
  GateTask(Gate* pGate, Context* pContext); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum TaskStates { OPEN, OPENING, CLOSED, CLOSING } state;
  long stateTimestamp;
  bool justEntered;

  Gate* pGate;
  Context* pContext;
};

#endif