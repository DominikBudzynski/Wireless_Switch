#ifndef _GPIO_SETUP_
#define _GPIO_SETUP_

#define LED_PIN GPIO_NUM_0
#define LD_PIN GPIO_NUM_15
#define RELAY_PIN GPIO_NUM_12
#define LD_POWER_PIN GPIO_NUM_2

void setupPins();
void changeRelayState(int value);

#endif