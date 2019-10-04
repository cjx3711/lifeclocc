#include "functions.h"
#include <arduino.h>

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

void blankScreen();
void initVariables() {
  counter = 1698765432;
  // Invert all the bits. This is needed if we are using common cathode.
  for (int i = 0; i < 10; i++) {
    NUMBER_CODE[i] = NUMBER_CODE[i] ^ B11111111;
    NUMBER_CODE_DP[i] = NUMBER_CODE_DP[i] ^ B11111111;
  }

  programState = STATE_CLOCK;
  programSubState = 0;
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
  for ( int i = 0; i < 4; i++ )
  longPressMills[i] = 0; // Used for timing long presses
  programSubState = 0; // Used for the state within the state
  // timeoutMills = 0; // Used for timeouts and reverting to base state
}

void timerPreLoop() {
  prevMills = currentMills;
  currentMills = millis();
  millsDelta = currentMills - prevMills;
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
