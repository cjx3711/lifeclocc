#include "functions.h"

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [3] = {false, false, false};
bool buttonStates [3] = {false, false, false};

long counter;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
unsigned long longPressMills;
unsigned long longPress2Mills;
unsigned long timeoutMills;
bool blinkPhase;

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

uint8_t stateSet;
uint8_t stateCounter;

void setup() {
  longPressMills = 0;
  longPress2Mills = 0;
  timeoutMills = 0;

  currentMinute = 0;
  currentHour = 0;
  currentSecond = 0;

  stateSet = 0;
  stateCounter = 0;

  // Initialise the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  pinMode(SET_LED_PIN, OUTPUT);
  pinMode(DATE_LED_PIN, OUTPUT);
  pinMode(TIME_LED_PIN, OUTPUT);
  pinMode(BIRTHDAY_LED_PIN, OUTPUT);
  setupBlink();
  // Initialise the pushbutton pins as an input:
  pinMode(BTN_UP_PIN, INPUT);
  pinMode(BTN_DOWN_PIN, INPUT);
  pinMode(BTN_RESET_PIN, INPUT);

  // Initialise bus pins
  pinMode(BCDA_PIN, OUTPUT);
  pinMode(BCDB_PIN, OUTPUT);
  pinMode(BCDC_PIN, OUTPUT);
  pinMode(BCDD_PIN, OUTPUT);

  // Initialise all the control pins
  for (int i = 0 ; i < 10 ; i++) {
     pinMode(FIRST_PIN + i, OUTPUT);
     digitalWrite(FIRST_PIN + i, HIGH);
  }

  // Serial.begin(9600);
  numberToDisplay(counter);

  setupBlink();

  eeprom_read_block((void*)&birthDate, (void*)0, sizeof(birthDate));

  if (!validDate(birthDate)) {
    birthDate.date = 1;
    birthDate.month = 1;
    birthDate.year = 1990;
    eeprom_write_block((const void*)&birthDate, (void*)0, sizeof(birthDate));
  } else {
    // Serial.println(birthDate.date);
    // Serial.println(birthDate.month);
    // Serial.println(birthDate.year);
  }

  setupBlink();
}

void loop() {
  currentMills = millis();
  millsDelta = currentMills - prevMills;
  prevMills = currentMills;
  timeoutMills += millsDelta;
  blinkPhase = (currentMills / BLINK_MS) % 2;

  digitalWrite(LED_PIN, LOW);
  buttonStates[BTN_UP] = digitalRead(BTN_UP_PIN);
  buttonStates[BTN_DOWN] = digitalRead(BTN_DOWN_PIN);
  buttonStates[BTN_RESET] = digitalRead(BTN_RESET_PIN);

  digitalWrite(LED_PIN, LOW);
  analogWrite(SET_LED_PIN, 0);
  analogWrite(DATE_LED_PIN, 0);
  analogWrite(TIME_LED_PIN, 0);
  analogWrite(BIRTHDAY_LED_PIN, 0);

  if (buttonStates[BTN_RESET]) {
    digitalWrite(LED_PIN, HIGH);
    longPressMills += millsDelta;
  } else {
    longPressMills = 0;
  }

  if (buttonStates[BTN_UP] || buttonStates[BTN_DOWN]) {
    digitalWrite(LED_PIN, HIGH);
    longPress2Mills += millsDelta;
  } else {
    longPress2Mills = 0;
  }

  if ( stateSet == 0 ) {
    // ----- REGULAR CLOCK STATE -------
    if (longPressMills > LONG_PRESS_TIMEOUT) {
      stateSet = 1;
      longPressMills = 0;
      stateCounter = 0;
      timeoutMills = 0;
    }

    if (longPress2Mills > LONG_PRESS_TIMEOUT) {
      stateSet = 2;
      longPressMills = 0;
      stateCounter = 0;
      timeoutMills = 0;
    }

    if (buttonRelease(BTN_UP)) {
      if (stateCounter != 2) stateCounter = 2;
      else stateCounter = 0;
    }

    if (buttonRelease(BTN_DOWN)) {
      if (stateCounter != 1) stateCounter = 1;
      else stateCounter = 0;
    }

    if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
    if (timeoutMills > CLOCK_STATE_TIMEOUT) {
      stateCounter = 0;
    }

    getTime();
    counter = getSecondsTillDeath();
    switch(stateCounter) {
      case 0:
        numberToDisplay(counter); break;
      case 1:
        analogWrite(DATE_LED_PIN, 128);
        dateToDisplay(currentDate.date, currentDate.month, currentDate.year + 1970, 0); break;
      case 2:
        analogWrite(TIME_LED_PIN, 128);
        timeToDsplay(currentHour, currentMinute, currentSecond, 0); break;
    }
  } else if (stateSet == 1) {
    analogWrite(SET_LED_PIN, 128);
    // ----- SETTING DATE STATE -----
    getTime();
    // printTime();
    bool timeChanged = false;
    uint16_t maxDays = daysInMonth(currentDate.month, currentDate.year + 1970);
    switch(stateCounter) {
      case 0: // Day
      case 1: // To allow for the first button release
        timeChanged = userModifyVariable(currentDate.date, 1, maxDays); break;
      case 2: // Month
        timeChanged = userModifyVariable(currentDate.month, 1, 12); break;
      case 3: // Year
        timeChanged = userModifyVariable(currentDate.year, 0, 150); break;
      case 4: // Hour
        timeChanged = userModifyVariable(currentHour, 0, 23); break;
      case 5: // Minute
        timeChanged = userModifyVariable(currentMinute, 0, 59); break;
      case 6: // Second
        timeChanged = userModifyVariable(currentSecond, 0, 59); break;
    }

    maxDays = daysInMonth(currentDate.month, currentDate.year + 1970);
    if (currentDate.date > maxDays) currentDate.date = maxDays;

    if (timeChanged) setTime();

    if ( buttonRelease(BTN_RESET) ) {
      stateCounter++;
      if (stateCounter > 6) stateCounter = 1;
    }

    if (longPressMills > LONG_PRESS_TIMEOUT) {
      longPressMills = 0;
      stateSet = 0;
      stateCounter = 0;
    }

    if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
    if (timeoutMills > SET_STATE_TIMEOUT) {
      stateSet = false;
    }

    if (stateCounter <= 3) {
      if ( blinkPhase ) analogWrite(DATE_LED_PIN, 128);
      dateToDisplay(currentDate.date, currentDate.month, currentDate.year + 1970, stateCounter == 0 ? 1 : stateCounter);
    } else {
      if ( blinkPhase ) analogWrite(TIME_LED_PIN, 128);
      timeToDsplay(currentHour, currentMinute, currentSecond, stateCounter - 3);
    }
  } else if (stateSet == 2) {
    analogWrite(SET_LED_PIN, 128);
    analogWrite(BIRTHDAY_LED_PIN, 128);
    // ------- SETTING BIRTHDAY STATE --------
    bool timeChanged = false;
    uint16_t maxDays = daysInMonth(birthDate.month, birthDate.year);
    switch(stateCounter) {
      case 0: // Day
        timeChanged = userModifyVariable(birthDate.date, 1, maxDays); break;
      case 1: // Month
        timeChanged = userModifyVariable(birthDate.month, 1, 12); break;
      case 2: // Year
        timeChanged = userModifyVariable(birthDate.year, 1900, 2100); break;
    }

    maxDays = daysInMonth(birthDate.month, birthDate.year);
    if (birthDate.date > maxDays) birthDate.date = maxDays;
    if (timeChanged) eeprom_write_block((const void*)&birthDate, (void*)0, sizeof(birthDate));

    dateToDisplay(birthDate.date, birthDate.month, birthDate.year, stateCounter + 1);

    if ( buttonRelease(BTN_RESET) ) {
      stateCounter++;
      if (stateCounter >= 3) stateCounter = 0;
    }

    if (longPressMills > LONG_PRESS_TIMEOUT) {
      longPressMills = 0;
      stateSet = 0;
      stateCounter = 0;
    }

    if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
    if (timeoutMills > SET_STATE_TIMEOUT) {
      stateSet = false;
    }
  }


  for ( int i = 0; i < 3; i++ ) {
    buttonStatesPrev[i] = buttonStates[i];
  }

  delay(10);
}
