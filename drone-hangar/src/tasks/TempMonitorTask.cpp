#include "tasks/TempMonitorTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

TempMonitorTask::TempMonitorTask(TempSensorTMP36* pTmp, Context* pContext): 
    pTmp(pTmp), pContext(pContext){
    resetTask();
}
  
void TempMonitorTask::tick(){
    switch (state){   
        case TaskStates::NORMAL: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[TMT] NORMAL"));
                pContext->setTempState(Context::TempStates::NORMAL);
            }

            float temp = pTmp->getTemperature();
            pContext->temperature = temp;
            if(temp >= TEMP_PRE_ALARM){
                setState(TaskStates::PRE_ALARM_DETECTION);
            } else if (temp > TEMP_ALARM){
                setState(TaskStates::ALARM_DETECTION);
            } 
            break;
        }
        case TaskStates::PRE_ALARM_DETECTION: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[TMT] PRE-ALARM DETECTION"));
            }

            float temp = pTmp->getTemperature();
            pContext->temperature = temp;
            if (temp < TEMP_PRE_ALARM){
                setState(TaskStates::NORMAL);
            } else if (temp > TEMP_ALARM){
                setState(TaskStates::ALARM_DETECTION);
            } else if (elapsedTimeInState() > TEMP_PRE_ALARM_TIME){
                setState(TaskStates::PRE_ALARM);
            }
            break;
        }
        case TaskStates::PRE_ALARM: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[TMT] PRE-ALARM"));
                pContext->setTempState(Context::TempStates::PRE_ALARM);
            }

            float temp = pTmp->getTemperature();
            pContext->temperature = temp;
            if(temp < TEMP_PRE_ALARM){
                setState(TaskStates::NORMAL);
            } else if (temp > TEMP_ALARM){
                setState(TaskStates::ALARM_DETECTION);
            }
            break;
        }
        case TaskStates::ALARM_DETECTION: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[TMT] PRE-ALARM DETECTION"));
            }

            float temp = pTmp->getTemperature();
            pContext->temperature = temp;
            if(temp < TEMP_PRE_ALARM){
                setState(TaskStates::NORMAL);
            }
            else if(temp < TEMP_ALARM){
                setState(TaskStates::PRE_ALARM);
            }
            else if(elapsedTimeInState() > TEMP_ALARM_TIME){
                setState(TaskStates::ALARM);
            }
            break;
        }
        case TaskStates::ALARM: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[TMT] ALARM"));
                pContext->setTempState(Context::TempStates::ALARM);
            }

            if(pContext->isResetRequested()){
                resetTask();
            }
            break;
        }
    }
}


void TempMonitorTask::resetTask(){
    setState(TaskStates::NORMAL);
    pContext->setTempState(Context::TempStates::NORMAL);
}

void TempMonitorTask::setState(int s){
    state = (TaskStates)s;
    stateTimestamp = millis();
    justEntered = true;
}

long TempMonitorTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool TempMonitorTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
