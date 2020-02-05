#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Time.h> // Time by Michael Margolis v 1.6.0
#include <Wire.h>
#include <DS1307RTC.h> // DS1370RTC by Michael Margolis v 1.4.1
#include <avr/eeprom.h>
#include "datatypes.h"
#include "constants.h"

extern const bool DEBUG;

extern bool buttonStatesPrev [];
extern bool buttonStates [];
extern unsigned long longPressMills [];

extern unsigned long counter;
extern unsigned long millsDelta;
extern unsigned long prevMills;
extern unsigned long currentMills;
extern unsigned long timeoutMills;

extern bool blinkPhase, prevBlinkPhase, blinkPhaseChange;
extern bool repeatPhase, prevRepeatPhase, repeatPhaseChange;

// Working variables
extern uint8_t digit;

// Date calculation workings
extern tmElements_t tm;
extern tmElements_t tm2;

extern SDate currentDate;
extern SDate birthDate;

extern uint16_t currentMinute;
extern uint16_t currentHour;
extern uint16_t currentSecond;

extern uint8_t programState;
extern uint8_t programSubState;



// =============================== INPUT HELPERS ============================
bool anyButtonRelease();
bool buttonRelease(uint8_t btn);
bool buttonPress(uint8_t btn);
bool userModifyVariable(uint16_t &var, uint16_t min, uint16_t max);


// =============================== DISPLAY HELPERS ============================
void numberToDisplay(unsigned long number);
void timeToDsplay(uint16_t h, uint16_t m, uint16_t s, uint8_t blinkWhich);
void dateToDisplay(uint16_t d, uint16_t m, uint16_t y, uint8_t blinkWhich);

void setDigit(uint8_t which, uint8_t number);
void setBlank(uint8_t which);
void writeBlankToBus();
void writeDigitToBus(uint8_t d);

void splashScreen();
void blankScreen();
void debugScreen();

// =============================== DATE HELPERS ============================
bool validDate(SDate date);
uint16_t daysInMonth(uint16_t month, uint16_t year);
void print2digits(int number);
void printTime();
void printTime(tmElements_t time);
void getTime();
void setTime();
unsigned long getSecondsTillDeath();

// ================================ SETUP ======================================
void setupBlink();
void initVariables();
void initPins();
void readBirthday();

// ================================ STATES =====================================
void preLoop();
void buttonStatePreLoop();
void buttonStatePostLoop();

void changeState(uint8_t state);

void stateClock();
void stateSetClock();
void stateSetBirthday();
void stateDebug();

#endif
