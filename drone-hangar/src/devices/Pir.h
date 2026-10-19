#ifndef __PIR__
#define __PIR__

#include "PresenceSensor.h"

class Pir: public PresenceSensor {
 
public: 
  Pir(int pin);
  bool isDetected();
  void calibrate();

  void sync();
  long getLastSyncTime();

protected: 
  void updateSyncTime(long time);

private:
  long lastTimeSync = -1;
  int pin;
  bool detected;
  
};

#endif
