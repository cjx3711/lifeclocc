#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Time.h> // Time by Michael Margolis v 1.6.0
#include <Wire.h>
#include <DS1307RTC.h> // DS1370RTC by Michael Margolis v 1.4.1
#include "datatypes.h"
#include "constants.h"

extern const bool DEBUG;

extern uint8_t NUMBER_CODE [];
extern uint8_t NUMBER_CODE_DP [];

extern bool buttonStatesPrev [];
extern bool buttonStates [];
extern unsigned long longPressMills [];

extern unsigned long counter;
extern unsigned long millsDelta;
extern unsigned long prevMills;
extern unsigned long currentMills;
extern unsigned long timeoutMills;

// Date calculation workings
extern tmElements_t tm;

extern uint16_t secondsToSubtract;

extern uint8_t programState;
extern uint8_t programSubState;


// ======================= INPUT HELPERS ========================
bool anyButtonRelease();
bool anyButtonPress();
bool buttonRelease(uint8_t btn);
bool buttonPress(uint8_t btn);

// ====================== DISPLAY HELPERS =======================

void numberToDisplay(unsigned long number, uint8_t);
void timeToDisplay(uint16_t h, uint16_t m, uint16_t s);
void dateToDisplay(uint16_t d, uint16_t m, uint16_t y);
void lineToDisplay();
void splashScreen();
void testScreen();
void blankScreen();


// ======================== DATE HELPERS =========================
void getTime();
void print2digits(int number);
void printTime(tmElements_t time);

// =========================== SETUP ==============================
void setupBlink();
void initVariables();
void initPins();

// ========================== STATES ============================
void changeState(uint8_t state);

void timerPreLoop();
void buttonStatePreLoop();
void buttonStatePostLoop();

void stateClock();
void stateSetClock();
void stateSetBirthday();
void stateDebug();
void stateGame();


#endif
