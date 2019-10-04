#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <Wire.h>
#include "datatypes.h"
#include "constants.h"

extern uint8_t NUMBER_CODE [];
extern uint8_t NUMBER_CODE_DP [];

extern bool buttonStatesPrev [];
extern bool buttonStates [];
extern unsigned long longPressMills [];

extern unsigned long counter;
extern unsigned long millsDelta;
extern unsigned long prevMills;
extern unsigned long currentMills;
extern uint16_t secondsToSubtract;

extern uint8_t programState;
extern uint8_t programSubState;


void numberToDisplay(unsigned long number, uint8_t);
void lineToDisplay();
// ========================== SETUP ============================
void setupBlink();
void splashScreen();
void testScreen();
void blankScreen();
void initVariables();
void initPins();

// ========================== STATES ============================
void changeState(uint8_t state);

void timerPreLoop();
void buttonStatePreLoop();
void buttonStatePostLoop();

#endif
