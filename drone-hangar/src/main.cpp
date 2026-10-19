#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/HangarTask.h"
#include "tasks/TempMonitorTask.h"
#include "tasks/LedTask.h"
#include "tasks/GateTask.h"

//#define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

void setup() {
  MsgService.init();
  sched.init(50);

  Logger.log(".::::: Drone Hangar :::::.");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pContext = new Context();

  // Hangar
  Task* pHangarTask = new HangarTask(pHWPlatform->getButton(), pHWPlatform->getLcd(), pHWPlatform->getPir(), pHWPlatform->getSonar(), pContext);
  pHangarTask->init(50); 

  // Temperature monitor system
  Task* pTempMonitorTask = new TempMonitorTask(pHWPlatform->getTempSensor(), pContext);
  pTempMonitorTask->init(250);

  // LEDs
  Task* pLedTask = new LedTask(pHWPlatform->getLed1(), pHWPlatform->getLed2(), pHWPlatform->getLed3(), pContext);
  pLedTask->init(250);

  // Gate
  Task* pGateTask = new GateTask(pHWPlatform->getGate(), pContext);
  pGateTask->init(50);
  
  sched.addTask(pHangarTask);
  sched.addTask(pTempMonitorTask);
  sched.addTask(pLedTask);
  sched.addTask(pGateTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(2000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    sched.schedule();
}