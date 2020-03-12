#include "functions.h"
const bool DEBUG = true;

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [4] = {false, false, false, false};
bool buttonStates [4] = {false, false, false, false};
unsigned long longPressMills [] = {0,0,0,0};

unsigned long counter;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
unsigned long timeoutMills;

bool blinkPhase, prevBlinkPhase, blinkPhaseChange;
bool repeatPhase, prevRepeatPhase, repeatPhaseChange;

// Date calculation workings
tmElements_t tm;

SDate currentDate;
STime currentTime;

uint16_t secondsToSubtract;

uint8_t programState;
uint8_t programSubState;

uint8_t NUMBER_CODE[] = {
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B10111110,
  B11100000,
  B11111110,
  B11110110,
};

uint8_t NUMBER_CODE_DP[] = {
  B11111101,
  B01100001,
  B11011011,
  B11110011,
  B01100111,
  B10110111,
  B10111111,
  B11100001,
  B11111111,
  B11110111,
};

void setup() {
  initPins();
  testScreen();
  setupBlink();
  initVariables();
  setupBlink();
  // delay(1000);
  // splashScreen();
  Serial.begin(9600);
  // setupBlink();
  // delay(3000);

  Serial.println("Lifeclocc Started");

  getTime();

  prevMills = currentMills = millis();
}


void loop() {
  getTime();
  timerPreLoop();
  buttonStatePreLoop();

  if ( secondsToSubtract > 1000 ) {
    counter--;
    secondsToSubtract -= 1000;
  }

  analogWrite(LED_BDAY_PIN, 0);
  analogWrite(LED_CLOCK_PIN, 0);
  analogWrite(LED_TIME_PIN, 0);
  analogWrite(LED_DATE_PIN, 0);

  if (digitalRead(BTN_BDAY_PIN)) analogWrite(LED_BDAY_PIN, 255);
  if (digitalRead(BTN_CLOCK_PIN)) analogWrite(LED_CLOCK_PIN, 255);
  if (digitalRead(BTN_TIME_PIN)) analogWrite(LED_TIME_PIN, 255);
  if (digitalRead(BTN_DATE_PIN)) analogWrite(LED_DATE_PIN, 255);

  int brightness = analogRead(POTIOMETER_PIN) * ((255.0f - MIN_BRIGHTNESS) / 1024.0f) + MIN_BRIGHTNESS;
  analogWrite(DSP_POWER_PIN, brightness);

  switch(programState) {
    case STATE_CLOCK:
      stateClock();
      break;
    case STATE_SET_CLOCK:
      stateSetClock();
      break;
    case STATE_SET_BIRTHDAY:
      stateSetBirthday();
      break;
    case STATE_DEBUG:
      stateDebug();
      break;
    case STATE_GAME:
      stateDebug();
      break;
  }

  buttonStatePostLoop();
  delay(50);
}
