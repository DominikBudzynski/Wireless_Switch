#include <gpioFunc.h>
#include <Arduino.h>

extern uint8_t relayState = 0;

void setupPins(){
    pinMode(LD_PIN, INPUT);    
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LD_POWER_PIN, OUTPUT);

    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LD_POWER_PIN, LOW);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void changeRelayState(int value){
    if (relayState == 1){
        if (value == 0){
            relayState = 0;
        }
    }
    if (relayState == 0){
        if (value == 1){
            relayState = 1;
        }
    }
}