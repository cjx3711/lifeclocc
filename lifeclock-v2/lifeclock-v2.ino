#include "functions.h"

// Settings are in the constants.h file
// // If it's common cathode, set to 0
// #define COMMON_ANODE 0
// // Display mode can either be SECONDS or DAYS
// #define DISPLAY_MODE WEEKS
// // Debug mode will print stuff
// #define DEBUG true

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [4] = {false, false, false, false};
bool buttonStates [4] = {false, false, false, false};
unsigned long longPressMills [] = {0,0,0,0};

unsigned long counter;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
unsigned long timeoutMills;

uint16_t loopInt = 0;

bool blinkPhase, prevBlinkPhase, blinkPhaseChange;
bool repeatPhase, prevRepeatPhase, repeatPhaseChange;

bool showDecisecond = true;

// Date calculation workings
tmElements_t tm;
tmElements_t tm2; // Stores birthday for subtraction

SDate currentDate;
SDate birthDate;
STime currentTime;

uint16_t secondsToSubtract;

uint8_t programState;
uint8_t programSubState;

uint8_t NUMBER_CODE[] = {
  DSP(0),
  DSP(1),
  DSP(2),
  DSP(3),
  DSP(4),
  DSP(5),
  DSP(6),
  DSP(7),
  DSP(8),
  DSP(9),
};

uint8_t ROTATION_CODE[] = {
  DSP(SEG_A),
  DSP(SEG_B),
  DSP(SEG_C),
  DSP(SEG_D),
  DSP(SEG_E),
  DSP(SEG_F)
};

uint8_t NUMBER_CODE_DP[] = {
  DSP(0_DP),
  DSP(1_DP),
  DSP(2_DP),
  DSP(3_DP),
  DSP(4_DP),
  DSP(5_DP),
  DSP(6_DP),
  DSP(7_DP),
  DSP(8_DP),
  DSP(9_DP),
};

SCharacter character;

void setup() {
  initPins();
  blankScreen();
  initVariables();
  for ( int i = 0; i < 4; i++ ) {
    setupBlink(LED_BDAY_PIN, 100, 0);
    setupBlink(LED_TIME_PIN, 100, 0);
    setupBlink(LED_DATE_PIN, 100, 0);
    setupBlink(LED_CLOCK_PIN, 100, 0);
  }
  splashScreen();
  blankScreen();
  Serial.begin(9600);
  readBirthday();
  setupBlink();
  timedTestScreen();

  Serial.println("Lifeclocc Started");

  getTime();

  prevMills = currentMills = millis();

  initCharacter();
}


void loop() {
  loopInt++;
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
      stateGame();
      break;
  }

  buttonStatePostLoop();
}
