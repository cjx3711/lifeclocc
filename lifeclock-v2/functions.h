#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Time.h> // Time by Michael Margolis v 1.6.0
#include <Wire.h>
#include <DS1307RTC.h> // DS1370RTC by Michael Margolis v 1.4.1
#include "datatypes.h"
#include "constants.h"

extern const bool DEBUG;
extern const bool DISPLAY_MODE;

extern uint8_t NUMBER_CODE [];
extern uint8_t NUMBER_CODE_DP [];
extern uint8_t ROTATION_CODE [];

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

extern bool showDecisecond;

// Date calculation workings
extern tmElements_t tm;
extern tmElements_t tm2; // Stores birthday for subtraction

extern SDate currentDate;
extern SDate birthDate;
extern STime currentTime;

extern uint16_t secondsToSubtract;

extern uint8_t programState;
extern uint8_t programSubState;

extern SCharacter character;


// ======================= INPUT HELPERS ========================
bool anyButtonRelease();
bool anyButtonPress();
bool buttonRelease(uint8_t btn);
bool buttonPress(uint8_t btn);
bool userModifyVariable(uint16_t &var, uint16_t min, uint16_t max);
bool userModifyVariable(bool &var);

// ====================== DISPLAY HELPERS =======================

void numberToDisplay(unsigned long number, uint8_t);
void twoNumbersToDisplay(unsigned long days, unsigned long seconds, uint8_t decimal);
void timeToDisplay(uint16_t h, uint16_t m, uint16_t s, uint8_t blinkWhich);
void dateToDisplay(uint16_t d, uint16_t m, uint16_t y, uint8_t blinkWhich);
void lineToDisplay();
void splashScreen();
void testScreen();
void blankScreen();


// ======================== DATE HELPERS =========================
void getTime();
void setTime();
unsigned long getSecondsTillDeath();
bool validDate(SDate date);
uint16_t daysInMonth(uint16_t month, uint16_t year);
void print2digits(int number);
void printTime(tmElements_t time);

// =========================== SETUP ==============================
void setupBlink();
void setupBlink(uint8_t whichLED, uint16_t onTime, uint16_t offTime);
void initVariables();
void initPins();
void readBirthday();

// ========================== STATES ============================
void changeState(uint8_t state);

void timerPreLoop();
void buttonStatePreLoop();
void buttonStatePostLoop();

void stateClock();
void stateSetClock();
void stateSetBirthday();
void stateDebug();

// =================== GAME STATE ====================
void initCharacter();
void stateGame();


#endif
