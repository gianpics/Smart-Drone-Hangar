#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

public:
  Context();

  // Reset
  bool isResetRequested();
  void setResetRequested(bool value);

  // Temperature
  float temperature;
  void setTempState(int state);
  int getTempState();
  bool isAlarmOn();
  bool isEmergency();
  float getTemperature();

  // Hangar
  void setHangarState(int state);
  int getHangarState();
  bool isTakingOff();
  bool isLanding();
  bool afterTookOff();
  bool afterLanding();
  bool mustOpenGate();
  bool mustCloseGate();

  // Gate
  void setGateState(int state);
  int getGateState();
  bool isGateClosed();
  bool isGateOpen();

  enum TempStates { NORMAL = 0, PRE_ALARM = 1, ALARM = 2 };
  enum HangarStates { IDLE, TAKE_OFF, FINALIZE_TAKE_OFF, LANDING, FINALIZE_LANDING, EMERGENCY };
  enum GateStates { CLOSED, CLOSING, OPEN, OPENING };

private:

  Context::TempStates tempState;
  Context::HangarStates hangarState;
  Context::GateStates gateState;
  bool resetRequested;

};

#endif
