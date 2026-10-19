#include "Context.h"

Context::Context(){
  resetRequested = false;
}

void Context::setTempState(int state){
  tempState = (TempStates)state;
}

int Context::getTempState(){
  return tempState;
}

bool Context::isAlarmOn(){
  return tempState > Context::TempStates::NORMAL;
}

bool Context::isEmergency()
{
  return tempState == Context::TempStates::ALARM && !resetRequested;
}

float Context::getTemperature()
{
  return temperature;
}

void Context::setHangarState(int state)
{
  hangarState = (HangarStates)state;
}

int Context::getHangarState()
{
    return hangarState;
}

bool Context::isTakingOff()
{
  return hangarState == Context::HangarStates::TAKE_OFF;
}

bool Context::isLanding(){
  return hangarState == Context::HangarStates::LANDING;
}

bool Context::afterTookOff(){
  return hangarState == Context::HangarStates::FINALIZE_TAKE_OFF;
}

bool Context::afterLanding(){
  return hangarState == Context::HangarStates::FINALIZE_LANDING;
}

bool Context::mustOpenGate()
{
  return isTakingOff() || isLanding();
}

bool Context::mustCloseGate()
{
  return afterTookOff() || afterLanding() || isEmergency();
}

void Context::setGateState(int state){
  gateState = (GateStates)state;
}

int Context::getGateState(){
  return gateState;
}

bool Context::isGateClosed(){
  return gateState == Context::GateStates::CLOSED;
}

bool Context::isGateOpen(){
  return gateState == Context::GateStates::OPEN;
}

bool Context::isResetRequested(){
  return resetRequested;
}

void Context::setResetRequested(bool value){
  resetRequested = value;
}
