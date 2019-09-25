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
  digitalWrite(BDAY_LED_PIN, HIGH);
  digitalWrite(CLOCK_LED_PIN, HIGH);
  digitalWrite(TIME_LED_PIN, HIGH);
  digitalWrite(DATE_LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BDAY_LED_PIN, LOW);
  digitalWrite(CLOCK_LED_PIN, LOW);
  digitalWrite(TIME_LED_PIN, LOW);
  digitalWrite(DATE_LED_PIN, LOW);
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
}
void initPins() {
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  pinMode(BDAY_LED_PIN, OUTPUT);
  pinMode(CLOCK_LED_PIN, OUTPUT);
  pinMode(TIME_LED_PIN, OUTPUT);
  pinMode(DATE_LED_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  pinMode(BDAY_BTN_PIN, INPUT);
  pinMode(CLOCK_BTN_PIN, INPUT);
  pinMode(TIME_BTN_PIN, INPUT);
  pinMode(DATE_BTN_PIN, INPUT);

  analogWrite(DSP_POWER_PIN, 128);
}

// ========================== STATES ============================
