#include "functions.h"
#include <arduino.h>

// ======================= INPUT HELPERS ========================

bool anyButtonPress() {
  return buttonPress(BTN_UP) || buttonPress(BTN_DOWN) || buttonPress(BTN_PREV) || buttonPress(BTN_NEXT);
}

bool anyButtonRelease() {
  return buttonRelease(BTN_UP) || buttonRelease(BTN_DOWN) || buttonRelease(BTN_PREV) || buttonRelease(BTN_NEXT);
}
bool buttonRelease(uint8_t btn) {
  return !buttonStates[btn] && buttonStatesPrev[btn];
}

bool buttonPress(uint8_t btn) {
  return buttonStates[btn] && !buttonStatesPrev[btn];
}

// ====================== DISPLAY HELPERS =======================


void numberToDisplay(unsigned long number, uint8_t decimal) {
  unsigned long workingCounter = number;

  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  // Send out the number after the decimal first.
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[decimal]);

  // Send the number with the decimal digit
  uint8_t digit = workingCounter % 10;
  workingCounter = workingCounter / 10;
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);

  for ( int i = 0; i < 9; i++ ) {
    digit = workingCounter % 10;
    workingCounter = workingCounter / 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void timeToDisplay(uint16_t h, uint16_t m, uint16_t s, uint8_t blinkWhich) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  uint8_t digit;
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);


  // Second
  if (blinkWhich == 3 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = s % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = s / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }

  // Minute
  if (blinkWhich == 2 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = m % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    digit = m / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }


  // Hour
  if (blinkWhich == 1 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = h % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    digit = h / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }
  
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);


  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void dateToDisplay(uint16_t y, uint16_t m, uint16_t d, uint8_t blinkWhich) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  uint8_t digit;

  // Day
  if (blinkWhich == 3 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = d % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = d / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);

  // Month
  if (blinkWhich == 2 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = m % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = m / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);

  // Year
  if (blinkWhich == 1 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  } else {
    digit = y % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = y / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = y / 100 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = y / 1000 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }
  
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);


  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void lineToDisplay() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);

  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}
  
  
void splashScreen() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_C);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_C);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_O_DP);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_L);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_C);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_E);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_F);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_I);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_L);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_DASH);

  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void testScreen() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  for ( uint8_t i = 0; i < 11; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_FULL);
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void blankScreen() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  for ( uint8_t i = 0; i < 11; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, CA_BLANK);
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

// ======================== DATE HELPERS =========================

void getTime() {
  if (RTC.read(tm)) {
    Serial.print("Time Read Successfully");
    printTime(tm);
  } else {
    Serial.print("Time Error");
  }
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void printTime(tmElements_t time) {
  if (!DEBUG) return;
  Serial.print("Time = ");
  print2digits(time.Hour);
  Serial.write(':');
  print2digits(time.Minute);
  Serial.write(':');
  print2digits(time.Second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(time.Day);
  Serial.write('/');
  Serial.print(time.Month);
  Serial.write('/');
  Serial.print(time.Year);
  Serial.println();
}

// ========================== SETUP ============================
void setupBlink() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_BDAY_PIN, HIGH);
  digitalWrite(LED_CLOCK_PIN, HIGH);
  digitalWrite(LED_TIME_PIN, HIGH);
  digitalWrite(LED_DATE_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_BDAY_PIN, LOW);
  digitalWrite(LED_CLOCK_PIN, LOW);
  digitalWrite(LED_TIME_PIN, LOW);
  digitalWrite(LED_DATE_PIN, LOW);
  delay(150);
}



void initVariables() {
  counter = 1698765432;
  // Invert all the bits. This is needed if we are using common cathode.
  for (int i = 0; i < 10; i++) {
    NUMBER_CODE[i] = NUMBER_CODE[i] ^ B11111111;
    NUMBER_CODE_DP[i] = NUMBER_CODE_DP[i] ^ B11111111;
  }

  timeoutMills = 0;

  programState = STATE_CLOCK;
  programSubState = 0;

  blinkPhase = prevBlinkPhase = blinkPhaseChange = false;
  repeatPhase = prevRepeatPhase = repeatPhaseChange = false;
}

void initPins() {
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  pinMode(LED_BDAY_PIN, OUTPUT);
  pinMode(LED_CLOCK_PIN, OUTPUT);
  pinMode(LED_TIME_PIN, OUTPUT);
  pinMode(LED_DATE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  pinMode(BTN_BDAY_PIN, INPUT);
  pinMode(BTN_CLOCK_PIN, INPUT);
  pinMode(BTN_TIME_PIN, INPUT);
  pinMode(BTN_DATE_PIN, INPUT);

  analogWrite(DSP_POWER_PIN, 128);
}

// ========================== STATES ============================

void changeState(uint8_t state) {
  programState = state;
  for ( int i = 0; i < 4; i++ ) longPressMills[i] = 0; // Used for timing long presses
  programSubState = 0; // Used for the state within the state
  timeoutMills = 0; // Used for timeouts and reverting to base state
}

void timerPreLoop() {
  prevMills = currentMills;
  currentMills = millis();
  millsDelta = currentMills - prevMills;
  timeoutMills += millsDelta;
  blinkPhase = (currentMills / BLINK_MS) % 2;
  blinkPhaseChange = blinkPhase != prevBlinkPhase;
  prevBlinkPhase = blinkPhase;
  repeatPhase = (currentMills / REPEAT_MS) % 4;
  repeatPhaseChange = repeatPhase != prevRepeatPhase;
  prevRepeatPhase = repeatPhase;

  secondsToSubtract += millsDelta;
}

void buttonStatePreLoop() {
  buttonStates[BTN_UP]   = digitalRead(BTN_UP_PIN);
  buttonStates[BTN_DOWN] = digitalRead(BTN_DOWN_PIN);
  buttonStates[BTN_PREV] = digitalRead(BTN_PREV_PIN);
  buttonStates[BTN_NEXT] = digitalRead(BTN_NEXT_PIN);

  // Global (not within states) input handlers
  digitalWrite(LED_PIN, LOW);
  for (int i = 0; i < 4; i++) {
    if (buttonStates[i]) {
      digitalWrite(LED_PIN, HIGH);
      longPressMills[i] += millsDelta;
    } else {
      longPressMills[i] = 0;
    }
  }
}

void buttonStatePostLoop() {
  for ( int i = 0; i < 4; i++ ) buttonStatesPrev[i] = buttonStates[i];
}


void stateClock() {

  if (longPressMills[BTN_UP] > SHORT_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > SHORT_PRESS_TIMEOUT && longPressMills[BTN_PREV] > SHORT_PRESS_TIMEOUT && longPressMills[BTN_NEXT] > SHORT_PRESS_TIMEOUT) {
    changeState(STATE_DEBUG);
  }

  if (longPressMills[BTN_UP] > LONG_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_GAME);
  }

  if (longPressMills[BTN_BDAY] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_BIRTHDAY);
  }

  if (longPressMills[BTN_CLOCK] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_CLOCK);
  }

  if (buttonRelease(BTN_TIME)) {
    if (programSubState != 2) programSubState = 2;
    else programSubState = 0;
  }

  if (buttonRelease(BTN_DATE)) {
    if (programSubState != 1) programSubState = 1;
    else programSubState = 0;
  }
  

  switch(programSubState) {
    case 0: // Regular Clock Mode
      numberToDisplay(counter, 9 - (secondsToSubtract / 100));
      break;
    case 1: // View Date Mode
      digitalWrite(LED_DATE_PIN, HIGH);

      dateToDisplay(tm.Year + 1970, tm.Month, tm.Day, 0);
      
      break;
    case 2: // View Time Mode
      digitalWrite(LED_TIME_PIN, HIGH);
      timeToDisplay(tm.Hour, tm.Minute, tm.Second, 0);
      break;
  }

}
void stateSetClock() {
  digitalWrite(LED_CLOCK_PIN, HIGH);

  // Input Handlers
  bool timeChanged = false;

  if (timeChanged) {
    // setTime();
    // printTime();
  }

  if (anyButtonRelease()) timeoutMills = 0;
  if (timeoutMills > SET_STATE_TIMEOUT) changeState(STATE_CLOCK);

  if ( buttonRelease(BTN_NEXT) ) {
    programSubState++;
    if (programSubState > 6) programSubState = 1;
  }

  if ( buttonRelease(BTN_PREV) ) {
    programSubState--;
    if (programSubState < 1) programSubState = 6;
  }
  
  if (longPressMills[BTN_NEXT] > LONG_PRESS_TIMEOUT || longPressMills[BTN_PREV] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }


  if (programSubState <= 3) {
    dateToDisplay(tm.Year + 1970, tm.Month, tm.Day, programSubState == 0 ? 1 : programSubState);
  } else {
    timeToDisplay(tm.Hour, tm.Minute, tm.Second, programSubState - 3);
  }
}
void stateSetBirthday() {
  digitalWrite(LED_BDAY_PIN, HIGH);
  lineToDisplay();

  if (longPressMills[BTN_BDAY] > LONG_PRESS_TIMEOUT || longPressMills[BTN_CLOCK] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  if (anyButtonRelease()) timeoutMills = 0;
  if (timeoutMills > 15000) changeState(STATE_CLOCK);
}
void stateDebug() {
  testScreen();
  digitalWrite(LED_DATE_PIN, HIGH);
  digitalWrite(LED_CLOCK_PIN, HIGH);
  digitalWrite(LED_TIME_PIN, HIGH);
  digitalWrite(LED_BDAY_PIN, HIGH);
  if (longPressMills[BTN_UP] > LONG_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  // Input Handlers
  // This ensures that all buttons are released first.
  // Then only on the second release, it changes back to the main state.
  if (anyButtonRelease()) {
    if (programSubState < 1) programSubState = 1;
    if (programSubState == 2) changeState(STATE_CLOCK);
  }
  if (anyButtonPress()) {
    if (programSubState == 1) programSubState = 2;
  }
}
void stateGame() {
  lineToDisplay();
  if (longPressMills[BTN_UP] > LONG_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }
}
