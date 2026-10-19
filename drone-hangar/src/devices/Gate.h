#ifndef __GATE_IMPL__
#define __GATE_IMPL__

#include <arduino.h>
#include "ServoMotorImpl.h"

class Gate: public ServoMotorImpl {

public:
  Gate(int pin);

  void tickOpen();
  bool isOpen();
  void tickClose();
  bool isClosed();
  
    
private:
  int position;
  long startTime;
  int startPosition;
  bool opening;

  long elapsedTimeFromStart();

  ServoMotorImpl* motor; 
};

#endif
