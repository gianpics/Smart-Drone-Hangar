#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "kernel/Task.h"
#include "devices/Button.h"
#include "devices/Lcd.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "model/Context.h"
#include "kernel/MsgService.h"
#include <Arduino.h>

#define DRONE_IN_TEXT "DRONE INSIDE"
#define TAKE_OFF_TEXT "TAKE OFF"
#define DRONE_OUT_TEXT "DRONE OUT"
#define LANDING_TEXT "LANDING"
#define ALARM_TEXT "ALARM"
#define LCD_ROW_POS 1
#define LCD_COL_POS 1

class HangarTask: public Task {

public:
  HangarTask(Button* pButton, Lcd* pLcd, Pir* pPir, Sonar* pSonar, Context* pContext); 
  void tick();
  void resetTask();


private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  bool checkAndSetJustEntered();
  
  const char* getHangarStateToString();
  const char* getDroneStateToString();

  void updateSerialDRU(float currentDistance = -1);
  void parseSerialDRU();
  bool requestedLandingDRU();
  bool requestedTakeOffDRU();
  bool noCommandReceivedDRU();
  void clearCommandDRU();
  bool isDroneHere();
  bool isDroneOutside();
  bool isDroneInside();
  bool isDroneBeyondSafeDistance(float currentDistance);
  bool isDistanceValid(float distance);
  void setDroneState(int state);
  
  enum CommandsDRU { CMD_TAKE_OFF, CMD_LANDING, CMD_NONE } commandReceived;
  enum Context::HangarStates state;
  long safeDistanceTimestamp;
  long stateTimestamp;
  bool justEntered;
  enum DroneStates { REST, OPERATING, TAKING_OFF, LANDING } droneState;

  Button* pButton;
  Lcd* pLcd;
  Pir* pPir;
  Sonar* pSonar;
  Context* pContext;
};

#endif