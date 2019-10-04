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

bool blinkPhase;
bool prevBlinkPhase;
bool blinkPhaseChange;

bool repeatPhase;
bool prevRepeatPhase;
bool repeatPhaseChange;

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

  // Read or initialise birthday
  eeprom_read_block((void*)&birthDate, (void*)0, sizeof(birthDate));

  if (!validDate(birthDate)) {
    birthDate.date = 1;
    birthDate.month = 1;
    birthDate.year = 1990;
    eeprom_write_block((const void*)&birthDate, (void*)0, sizeof(birthDate));
    if (DEBUG) Serial.println("Init Birthday: ");
  } else {
    if (DEBUG) {
      Serial.print("Read Birthday: "); Serial.print(birthDate.date);
      Serial.print(" "); Serial.print(birthDate.month);
      Serial.print(" "); Serial.println(birthDate.year);
    }
  }

  setupBlink();
  delay(200);
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
  }

  buttonStatePostLoop();
  delay(50);
}
