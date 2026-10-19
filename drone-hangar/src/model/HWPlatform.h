#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "devices/Lcd.h"
#include "devices/TempSensorTMP36.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/Gate.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  Led*  getLed1();
  Led*  getLed2();
  Led*  getLed3();
  Gate* getGate();
  Lcd* getLcd();
  TempSensorTMP36* getTempSensor();
  Sonar* getSonar();
  Pir* getPir();

private:
  Button* pButton;
  Led* pLed1;
  Led* pLed2;
  Led* pLed3;
  Gate* pGate;
  Lcd* pLcd;
  TempSensorTMP36* pTempSensor;
  Sonar* pSonar;
  Pir* pPir;
};

#endif
