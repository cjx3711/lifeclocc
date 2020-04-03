#include "functions.h"
const bool DEBUG = true;

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [3] = {false, false, false};
bool buttonStates [3] = {false, false, false};
unsigned long longPressMills [] = {0,0,0};

unsigned long counter;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
unsigned long timeoutMills;

bool blinkPhase, prevBlinkPhase, blinkPhaseChange;
bool repeatPhase, prevRepeatPhase, repeatPhaseChange;

// Working variables
uint8_t digit;

// Date calculation workings
tmElements_t tm;
tmElements_t tm2;

SDate currentDate;
SDate birthDate;

uint16_t currentMinute;
uint16_t currentHour;
uint16_t currentSecond;

uint8_t programState;
uint8_t programSubState;

void setup() {
  initVariables();
  initPins();

  blankScreen();
  setupBlink();
  if (DEBUG) Serial.begin(9600);

  splashScreen();
  setupBlink();

  readBirthday();

  setupBlink();
  delay(100);
}

void loop() {
  preLoop();
  buttonStatePreLoop();
  getTime();

  switch (programState) {
    case STATE_CLOCK:
      // ------- REGULAR CLOCK STATE -------
      stateClock();
      break;
    case STATE_SET_CLOCK:
      // ------- SETTING DATE STATE -------
      stateSetClock();
      break;
    case STATE_SET_BIRTHDAY:
      // ------- SETTING BIRTHDAY STATE --------
      stateSetBirthday();
      break;
    case STATE_DEBUG:
      // ------- DEBUG STATE ---------
      stateDebug();
      break;
  }

  buttonStatePostLoop();
  delay(50);
}
