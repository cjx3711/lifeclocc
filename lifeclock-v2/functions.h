#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <Wire.h>
#include "datatypes.h"
#include "constants.h"

extern uint8_t NUMBER_CODE [];
extern uint8_t NUMBER_CODE_DP [];

extern bool buttonStatesPrev [];
extern bool buttonStates [];

extern unsigned long counter;
extern unsigned long millsDelta;
extern unsigned long prevMills;
extern unsigned long currentMills;

void numberToDisplay(unsigned long number, uint8_t);

// ========================== SETUP ============================
void setupBlink();
void splashScreen();
void testScreen();
void blankScreen();
void initVariables();
void initPins();

// ========================== STATES ============================
void buttonStatePreLoop();
void buttonStatePostLoop();

#endif
