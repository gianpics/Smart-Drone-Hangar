#include "tasks/GateTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

GateTask::GateTask(Gate* pGate, Context* pContext): 
    pGate(pGate), pContext(pContext){
    setState(TaskStates::CLOSED);
}
  
void GateTask::tick(){
    switch (state){ 
        case TaskStates::OPEN: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[GT] OPEN"));
                pContext->setGateState(Context::GateStates::OPEN);
            }

            if(pContext->mustCloseGate()) {
                setState(TaskStates::CLOSING);
            }
            break; 
        }
        case TaskStates::OPENING: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[GT] OPENING"));
                pContext->setGateState(Context::GateStates::OPENING);
            }

            if(pContext->mustCloseGate()){
                setState(TaskStates::CLOSING);
                return;
            }

            pGate->tickOpen();

            if(pGate->isOpen()){
                setState(TaskStates::OPEN);
            }
            break;
        }
        case TaskStates::CLOSED: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[GT] CLOSED"));
                pContext->setGateState(Context::GateStates::CLOSED);
            }

            if(pContext->mustOpenGate()){
                setState(TaskStates::OPENING);
            }
            break;
        }
        case TaskStates::CLOSING: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[GT] CLOSING"));
                pContext->setGateState(Context::GateStates::CLOSING);
            }

            pGate->tickClose();

            if(pGate->isClosed()) {
                setState(TaskStates::CLOSED);
            }
            break;
        }
    }
}

void GateTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long GateTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool GateTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
