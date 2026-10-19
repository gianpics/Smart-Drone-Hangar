#ifndef __LED_TASK__
#define __LED_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#include <Arduino.h>

class LedTask: public Task {

public:
  LedTask(Led* pLed1, Led* pLed2, Led* pLed3, Context* pContext); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  bool mustBlink();
  
  enum TaskStates { IDLE, BLINK_OFF, BLINK_ON, ALARM } state;
  long stateTimestamp;
  bool justEntered;

  Led* pLed1; // GREEN
  Led* pLed2; // GREEN
  Led* pLed3; // RED
  Context* pContext;
};

#endif