#include "HangarTask.h"
#include <Arduino.h>
#include "config.h"
#include <math.h>
#include "kernel/Logger.h"
 
HangarTask::HangarTask(Button* pButton, Lcd* pLcd, Pir* pPir, Sonar* pSonar, Context* pContext): 
    pButton(pButton), pLcd(pLcd), pPir(pPir), pSonar(pSonar), pContext(pContext){
    resetTask();
}
  
void HangarTask::tick(){
    switch (state){    
        case Context::HangarStates::IDLE: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] IDLE"));
                if(isDroneInside()){
                    pLcd->display(DRONE_IN_TEXT, LCD_ROW_POS, LCD_COL_POS);
                }
                else{
                    pLcd->display(DRONE_OUT_TEXT, LCD_ROW_POS, LCD_COL_POS);
                }
                clearCommandDRU();
                updateSerialDRU();
            }

            if(pContext->isAlarmOn()) {
                if(pContext->isEmergency() && !pContext->isResetRequested()){
                    setState(Context::HangarStates::EMERGENCY);   
                }
                return;
            }

            if(noCommandReceivedDRU()){
                parseSerialDRU();
            }

            if(requestedTakeOffDRU() && isDroneInside()){
                setState(Context::HangarStates::TAKE_OFF);
            } else if(requestedLandingDRU() && isDroneOutside() && isDroneHere()){
                setState(Context::HangarStates::LANDING);
            }
            break;
        }
        case Context::HangarStates::TAKE_OFF: { 
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] TAKE_OFF"));
                pLcd->display(TAKE_OFF_TEXT, LCD_ROW_POS, LCD_COL_POS);
                setDroneState(DroneStates::TAKING_OFF);
                updateSerialDRU();
                safeDistanceTimestamp = -1;
            }

            if(pContext->isEmergency()){
                setState(Context::HangarStates::EMERGENCY);
                return;
            }

            float currentDistance = pSonar->getDistance();

            if(pContext->isGateOpen() && isDroneBeyondSafeDistance(currentDistance)){
                setState(Context::HangarStates::FINALIZE_TAKE_OFF);
            }

            break; 
        }
        case Context::HangarStates::FINALIZE_TAKE_OFF: { 
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] FINALIZING_TAKE_OFF"));
                setDroneState(DroneStates::OPERATING);
                updateSerialDRU();
            }

            if(pContext->isEmergency()){
                setState(Context::HangarStates::EMERGENCY);
                return;
            }

            if(pContext->isGateClosed()){
                setState(Context::HangarStates::IDLE);
            }

            break; 
        }
        case Context::HangarStates::LANDING: { 
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] LANDING"));
                pLcd->display(LANDING_TEXT, LCD_ROW_POS, LCD_COL_POS);
                setDroneState(DroneStates::LANDING);
                safeDistanceTimestamp = -1;
            }

            if(pContext->isEmergency()){
                setState(Context::HangarStates::EMERGENCY);
                return;
            }

            float currentDistance = pSonar->getDistance();

            updateSerialDRU(currentDistance);

            if(pContext->isGateOpen() && isDroneBeyondSafeDistance(currentDistance)){
                setState(Context::HangarStates::FINALIZE_LANDING);
            }

            break; 
        }
        case Context::HangarStates::FINALIZE_LANDING: { 
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] FINALIZING_LANDING"));
                setDroneState(DroneStates::REST);
                updateSerialDRU();
            }

            if(pContext->isEmergency()){
                setState(Context::HangarStates::EMERGENCY);
                return;
            }

            if(pContext->isGateClosed()){
                setState(Context::HangarStates::IDLE);
            }

            break; 
        }
        case Context::HangarStates::EMERGENCY: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[HT] EMERGENCY"));
                pLcd->display(ALARM_TEXT, LCD_ROW_POS, LCD_COL_POS);
                updateSerialDRU();
            }

            if(pButton->isPressed()){
                pContext->setResetRequested(true);
                resetTask();
            }

            break;
        }
    }
}

void HangarTask::resetTask()
{
    setState(Context::HangarStates::IDLE);
    setDroneState(DroneStates::REST);
    clearCommandDRU();
}

void HangarTask::setState(int s){
    state = (Context::HangarStates)s;
    pContext->setHangarState((Context::HangarStates)s);
    stateTimestamp = millis();
    justEntered = true;
}

long HangarTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool HangarTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}

const char *HangarTask::getHangarStateToString()
{
    if(pContext->isEmergency() && isDroneOutside()){
        return "ALARM";
    }
    else{
        return "NORMAL";
    }
}

const char *HangarTask::getDroneStateToString()
{
    switch (droneState) {
        default:
        case DroneStates::REST:       return "REST";
        case DroneStates::OPERATING:  return "OPERATING";
        case DroneStates::TAKING_OFF: return "TAKING_OFF";
        case DroneStates::LANDING:    return "LANDING";
    }
}

void HangarTask::updateSerialDRU(float currentDistance)
{
    String msg = "<";
    msg += getDroneStateToString();
    msg += ";";
    msg += getHangarStateToString();
    msg += ";";

    if(currentDistance != -1){
        msg += String(currentDistance, 2);
    }

    msg += ">";
    MsgService.sendMsg(msg);
}

void HangarTask::parseSerialDRU()
{
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        String data = msg->getContent();
        delete msg; 
        data.trim();

        if (data == "TAKE_OFF") {
            Logger.log("Received TAKE_OFF command");
            commandReceived = CommandsDRU::CMD_TAKE_OFF;
        }
        else if (data == "LANDING") {
            Logger.log("Received LANDING command");
            commandReceived = CommandsDRU::CMD_LANDING;
        }
    }
}

void HangarTask::clearCommandDRU()
{
    commandReceived = CommandsDRU::CMD_NONE;
}

bool HangarTask::isDroneHere()
{
    long pirLastSyncTime = pPir->getLastSyncTime();

    if(pirLastSyncTime == -1 || (millis() - pirLastSyncTime) > PIR_SYNC_EXPIRATION){
        pPir->sync();
    }

    if(pPir->isDetected()){
        Logger.log("Drone detected");
    }
    return pPir->isDetected();
}

bool HangarTask::isDroneOutside()
{
  return droneState == DroneStates::OPERATING;
}

bool HangarTask::isDroneInside()
{
  return droneState == DroneStates::REST;
}

bool HangarTask::isDroneBeyondSafeDistance(float currentDistance)
{

    char charVal[10];                

    //4 is mininum width, 3 is precision; float value is copied onto buff
    dtostrf(currentDistance, 4, 3, charVal);
    //Logger.log(charVal);
    const float timeThreshold = state == Context::HangarStates::TAKE_OFF ? TAKE_OFF_SAFE_TIME : LANDING_SAFE_TIME;

    if(isDistanceValid(currentDistance)){
        long now = millis();
        if(safeDistanceTimestamp == -1){
            //Logger.log("timestamp starting now");
            safeDistanceTimestamp = now;
            return false;
        }
        if(now - safeDistanceTimestamp > timeThreshold){
            //Logger.log("Drone is beyond safe distance");
            return true;
        }
        return false;
    } else {
        //Logger.log("Drone is far");
        safeDistanceTimestamp = -1;
        return false;
    }
}

bool HangarTask::isDistanceValid(float distance){
    if(state == Context::HangarStates::TAKE_OFF){
        return distance > TAKE_OFF_SAFE_DISTANCE;
    }
    else{
        // Ignore zero and negative distance (no object)
        return distance > 0.0 && distance < LANDING_SAFE_DISTANCE;
    }
}

void HangarTask::setDroneState(int state)
{
    droneState = (DroneStates)state;
}

bool HangarTask::requestedLandingDRU()
{
    return commandReceived == CommandsDRU::CMD_LANDING;
}

bool HangarTask::requestedTakeOffDRU()
{
    return commandReceived == CommandsDRU::CMD_TAKE_OFF;
}

bool HangarTask::noCommandReceivedDRU()
{
    return commandReceived == CommandsDRU::CMD_NONE;
}
