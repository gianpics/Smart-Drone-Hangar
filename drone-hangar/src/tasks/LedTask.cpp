#include "tasks/LedTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "LedTask.h"

LedTask::LedTask(Led* pLed1, Led* pLed2, Led* pLed3, Context* pContext): 
    pLed1(pLed1), pLed2(pLed2), pLed3(pLed3), pContext(pContext){
    setState(TaskStates::IDLE);
}
  
void LedTask::tick(){
    switch (state){   
        case TaskStates::IDLE: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[LT] IDLE"));
                pLed1->switchOn();
                pLed2->switchOff();
                pLed3->switchOff();
            }
            
            if(pContext->isEmergency()){
                setState(TaskStates::ALARM);
            }
            else if(mustBlink()){
                setState(TaskStates::BLINK_OFF);
                Logger.log(F("[LT] BLINKING"));
            }
            break;
        }
        case TaskStates::BLINK_OFF: {
            if (this->checkAndSetJustEntered()){
                //Logger.log(F("[LT] BLINK_OFF"));
                pLed1->switchOff();
                pLed2->switchOff();
            }

            if(pContext->isEmergency()){
                setState(TaskStates::ALARM);
            }
            else if (mustBlink()){
                setState(TaskStates::BLINK_ON);
            } else {
                setState(TaskStates::IDLE);
            }
            break;
        }
        case TaskStates::BLINK_ON: {
            if (this->checkAndSetJustEntered()){
                //Logger.log(F("[LT] BLINK_ON"));
                pLed2->switchOn();
            }

            if(pContext->isEmergency()){
                setState(TaskStates::ALARM);
            }
            else if (mustBlink()){
                setState(TaskStates::BLINK_OFF);
            } else {
                setState(TaskStates::IDLE);
            }
            break;
        }
        case TaskStates::ALARM: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[LT] ALARM"));
                pLed1->switchOff();
                pLed2->switchOff();
                pLed3->switchOn();
            }
            
            if(!pContext->isEmergency()){
                setState(TaskStates::IDLE);
            }
            break;
        }
    }
}


void LedTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long LedTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool LedTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}

bool LedTask::mustBlink()
{
    return pContext->isTakingOff() 
            || pContext->isLanding()
            || pContext->afterTookOff()
            || pContext->afterLanding();
}
