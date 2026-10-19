#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/LightSensorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  pButton = new ButtonImpl(BTN_PIN);
  pLed1 = new Led(LED1_PIN);
  pLed2 = new Led(LED2_PIN);
  pLed3 = new Led(LED3_PIN);
  pGate = new Gate(MOTOR_PIN);
  pLcd = new Lcd(LCD_PORT, LCD_COLS_NUM, LCD_ROWS_NUM);
  pTempSensor = new TempSensorTMP36(TMP_PIN);
  pPir = new Pir(PIR_PIN);
  pSonar = new Sonar(ECHO_PIN, TRIG_PIN, SONAR_MAX_TIME);
}


void HWPlatform::init(){
  Logger.log("Calibrating PIR...");
  pGate->on();
  pGate->setPosition(0);
  pPir->calibrate();
  pSonar->setTemperature(pTempSensor->getTemperature());
  pGate->off();
}

Button* HWPlatform::getButton(){
  return this->pButton;
}

Led* HWPlatform::getLed1(){
  return this->pLed1;
}

Led* HWPlatform::getLed2(){
  return this->pLed2;
}

Led* HWPlatform::getLed3(){
  return this->pLed3;
}

Gate* HWPlatform::getGate(){
  return this->pGate;
}

Lcd* HWPlatform::getLcd(){
  return this->pLcd;
}

TempSensorTMP36* HWPlatform::getTempSensor(){
  return this->pTempSensor;
}

Sonar* HWPlatform::getSonar(){
  return this->pSonar;
}

Pir* HWPlatform::getPir(){
  return this->pPir;
}

void HWPlatform::test(){
  bool btPressed = pButton->isPressed();
  pPir->sync();
  bool isDetected = pPir->isDetected();
  float temperature = pTempSensor->getTemperature();
  float distance = pSonar->getDistance();

  Logger.log("LEDs ON");
  pLed1->switchOn();
  pLed2->switchOn();
  pLed3->switchOn();
  pGate->on();
  pGate->setPosition(GATE_OPENING_DEG);
  pLcd->display(LCD_TEST_STRING, 1, 1);

  Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  Logger.log("Temperature: " + String(temperature));
  Logger.log("Presence: " + String(isDetected ? "detected" : " not detected"));
  Logger.log("Distance: " + String(distance));
  
  delay(3000);
  pGate->setPosition(0);
  delay(GATE_OPENING_TIME);
  pGate->off();
  Logger.log("LEDs OFF");
  pLed1->switchOff();
  pLed2->switchOff();
  pLed3->switchOff();
  

  Logger.log("==========================");
}
