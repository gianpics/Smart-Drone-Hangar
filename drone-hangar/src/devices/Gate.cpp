#include "Gate.h"
#include "Arduino.h"
#include "config.h"

Gate::Gate(int pin): ServoMotorImpl(pin){
    setPosition(0);
    position = 0;
    opening = false;
    startPosition = 0;
}

void Gate::tickOpen(){
    if(!opening || !isOn()){
        // First tick of an opening movement
        opening = true;
        on();
        startTime = millis();
        startPosition = position;
    }
    
    long dt = elapsedTimeFromStart();
    float timeToOpen = (((float)(GATE_OPENING_DEG - startPosition)) / GATE_OPENING_DEG) * GATE_OPENING_TIME;

    if (timeToOpen <= 0) {
        position = GATE_OPENING_DEG;
    } else {
        float ratio = ((float)dt) / timeToOpen;
        if (ratio > 1.0f) ratio = 1.0f;
        float posF = startPosition + (GATE_OPENING_DEG - startPosition) * ratio;
        position = (int)posF;
    }
    setPosition(position);
}

void Gate::tickClose(){
    if(opening || !isOn()){
        // First tick of a closing movement
        opening = false;
        on();
        startTime = millis();
        startPosition = position;
    }

    long dt = elapsedTimeFromStart();
    float timeToClose = (((float)startPosition) / GATE_OPENING_DEG) * GATE_OPENING_TIME;

    if (timeToClose <= 0) {
        position = 0;
    } else {
        float ratio = ((float)dt) / timeToClose;
        if (ratio > 1.0f) ratio = 1.0f;
        float posF = startPosition * (1.0f - ratio);
        position = (int)posF;
    }
    setPosition(position);
    
    if(isClosed()){
        // final tick
        off();
    }
}

bool Gate::isOpen(){
    return position == GATE_OPENING_DEG;
}

bool Gate::isClosed(){
    return position == 0;
}

long Gate::elapsedTimeFromStart() {
    return millis() - startTime;
}