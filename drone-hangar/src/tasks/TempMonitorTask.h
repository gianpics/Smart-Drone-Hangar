#ifndef __TEMP_MONITOR_TASK__
#define __TEMP_MONITOR_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/TempSensorTMP36.h"
#include <Arduino.h>

/**
 * Class implementing the Temperature Monitoring system
 */
class TempMonitorTask: public Task {

public:
  TempMonitorTask(TempSensorTMP36* pTmp, Context* pContext); 
  void tick();
  void resetTask();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum TaskStates { NORMAL, PRE_ALARM_DETECTION, PRE_ALARM, ALARM_DETECTION, ALARM } state;
  long stateTimestamp;
  bool justEntered;

  TempSensorTMP36* pTmp;
  Context* pContext;
};

#endif