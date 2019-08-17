#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

#define LED_PIN 6

#define BCDA_PIN 23
#define BCDB_PIN 24
#define BCDC_PIN 25
#define BCDD_PIN 26

#define FIRST_PIN 8

#define SECONDS_IN_DAY 31536000
#define LONG_PRESS_TIMEOUT 2000
#define SET_STATE_TIMEOUT 15000
#define CLOCK_STATE_TIMEOUT 5000
#define BLINK_MS 500

#define BTN_UP_PIN 7
#define BTN_DOWN_PIN 5
#define BTN_RESET_PIN 4

#define BTN_UP 0
#define BTN_DOWN 1
#define BTN_RESET 2

uint8_t digit = 3;
uint8_t currentAge = 27;
uint8_t maxAge = 80;


// Working variables

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [3] = {false, false, false};
bool buttonStates [3] = {false, false, false};

long counter = (maxAge-currentAge) * SECONDS_IN_DAY + 12858;
long currentSecs = 0;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
unsigned long outstandingMills = 0;
unsigned long longPressMills = 0;
unsigned long longPress2Mills = 0;
unsigned long timeoutMills = 0;
bool blinkPhase;


// Date calculation workings
tmElements_t tm;

uint16_t currentDate = 24;
uint16_t currentMonth = 7;
uint16_t currentYear = 2019;

uint16_t currentMinute = 0;
uint16_t currentHour = 0;
uint16_t currentSecond = 0;

uint16_t birthDate = 29;
uint16_t birthMonth = 5;
uint16_t birthYear = 1992;

uint8_t stateSet = 0;
uint8_t stateCounter = 0;

void numberToDisplay(long number) {
  long workingCounter = number;
  for ( int i = 0; i < 10; i++ ) {
    digit = workingCounter % 10;
    workingCounter = workingCounter / 10;
    if ( workingCounter == 0 && digit == 0 ) setBlank(i);
    else setDigit(i, digit);
  }
}

void timeToDsplay(uint16_t h, uint16_t m, uint16_t s, uint8_t blinkWhich) {
  // Blank the display
  for (int i = 0; i < 10; i++ ) {
    setBlank(i);
  }

  uint16_t workings;
  // Set hours
  workings = h;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 1 && blinkPhase ) setBlank(i + 7);
    else setDigit(i + 7, digit);
  }

  // Set minutes
  workings = m;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 2 && blinkPhase ) setBlank(i + 4);
    else setDigit(i + 4, digit);
  }

  // Set seconds
  workings = s;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 3 && blinkPhase ) setBlank(i + 1);
    else setDigit(i + 1, digit);
  }
}
void dateToDisplay(uint16_t d, uint16_t m, uint16_t y, uint8_t blinkWhich) {
  // Blank the display
  for (int i = 0; i < 10; i++ ) {
    setBlank(i);
  }

  uint16_t workings;
  // Set date
  workings = d;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 1 && blinkPhase ) setBlank(i + 8);
    else setDigit(i + 8, digit);
  }

  // Set month
  workings = m;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 2 && blinkPhase ) setBlank(i + 5);
    else setDigit(i + 5, digit);
  }

  // Set year
  workings = y;
  for ( int i = 0; i < 4; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( blinkWhich == 3 && blinkPhase ) setBlank(i);
    else setDigit(i, digit);
  }
}


void setDigit(uint8_t which, uint8_t number) {
  digitalWrite(FIRST_PIN + which, LOW);
  writeDigitToBus(digit);
  digitalWrite(FIRST_PIN + which, HIGH);
}

void setBlank(uint8_t which) {
  digitalWrite(FIRST_PIN + which, LOW);
  writeBlankToBus();
  digitalWrite(FIRST_PIN + which, HIGH);
}

void writeBlankToBus() {
  digitalWrite(BCDA_PIN, 1);
  digitalWrite(BCDB_PIN, 1);
  digitalWrite(BCDC_PIN, 1);
  digitalWrite(BCDD_PIN, 1);
}

void writeDigitToBus(uint8_t d) {
  digitalWrite(BCDA_PIN, (d >> 0) & 1);
  digitalWrite(BCDB_PIN, (d >> 1) & 1);
  digitalWrite(BCDC_PIN, (d >> 2) & 1);
  digitalWrite(BCDD_PIN, (d >> 3) & 1);
}

bool anyButtonRelease() {
  return buttonRelease(BTN_UP) || buttonRelease(BTN_DOWN) || buttonRelease(BTN_RESET);
}
bool buttonRelease(uint8_t btn) {
  return !buttonStates[btn] && buttonStatesPrev[btn];
}

bool buttonPress(uint8_t btn) {
  return buttonStates[btn] && !buttonStatesPrev[btn];
}


bool userModifyVariable(uint16_t &var, uint16_t min, uint16_t max) {
  if (buttonPress(BTN_UP)) {
    if (var == max) var = min;
    else var++;
    return true;
  } else if(buttonPress(BTN_DOWN)) {
    if (var == min) var = max;
    else var--;
    return true;
  }
  return false;
}

void setup() {
  // Initialise the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
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

  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  numberToDisplay(counter);
}

void loop() {
  currentMills = millis();
  millsDelta = currentMills - prevMills;
  prevMills = currentMills;
  outstandingMills += millsDelta;
  timeoutMills += millsDelta;
  blinkPhase = (currentMills / BLINK_MS) % 2;

  while (outstandingMills > 1000) {
    outstandingMills -= 1000;
    counter--;
  }

  digitalWrite(LED_PIN, LOW);
  buttonStates[BTN_UP] = digitalRead(BTN_UP_PIN);
  buttonStates[BTN_DOWN] = digitalRead(BTN_DOWN_PIN);
  buttonStates[BTN_RESET] = digitalRead(BTN_RESET_PIN);

  digitalWrite(LED_PIN, LOW);

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

    if (buttonPress(BTN_UP)) {
      if (stateCounter == 2) stateCounter = 0;
      else stateCounter++;
    }

    if (buttonPress(BTN_DOWN)) {
      if (stateCounter == 0) stateCounter = 2;
      else stateCounter--;
    }

    if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
    if (timeoutMills > CLOCK_STATE_TIMEOUT) {
      stateCounter = 0;
    }

    switch(stateCounter) {
      case 0:
        numberToDisplay(counter); break;
      case 1:
        getTime();
        dateToDisplay(currentDate, currentMonth, currentYear + 1970, 0); break;
      case 2:
        getTime();
        timeToDsplay(currentHour, currentMinute, currentSecond, 0); break;
    }
  } else if (stateSet == 1) {
    // ----- SETTING DATE STATE -----
    getTime();
    // printTime();
    bool timeChanged = false;
    switch(stateCounter) {
      case 0: // Day
      case 1: // To allo for the first button release
        timeChanged = userModifyVariable(currentDate, 1, 31); break;
      case 2: // Month
        timeChanged = userModifyVariable(currentMonth, 1, 12); break;
      case 3: // Year
        timeChanged = userModifyVariable(currentYear, 0, 150); break;
      case 4: // Hour
        timeChanged = userModifyVariable(currentHour, 0, 23); break;
      case 5: // Minute
        timeChanged = userModifyVariable(currentMinute, 0, 59); break;
      case 6: // Second
        timeChanged = userModifyVariable(currentSecond, 0, 59); break;
    }

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


    if (stateCounter <= 3) dateToDisplay(currentDate, currentMonth, currentYear + 1970, stateCounter == 0 ? 1 : stateCounter);
    else timeToDsplay(currentHour, currentMinute, currentSecond, stateCounter - 3);
  } else if (stateSet == 2) {
    // ------- SETTING BIRTHDAY STATE --------
    bool timeChanged = false;
    switch(stateCounter) {
      case 0: // Day
        timeChanged = userModifyVariable(birthDate, 1, 31); break;
      case 1: // Month
        timeChanged = userModifyVariable(birthMonth, 1, 12); break;
      case 2: // Year
        timeChanged = userModifyVariable(birthYear, 1900, 2100); break;
    }

    dateToDisplay(birthDate, birthMonth, birthYear, stateCounter + 1);

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

void getTime() {
  if (RTC.read(tm)) {
    currentDate = tm.Day;
    currentMonth = tm.Month;
    currentYear = tm.Year;
    currentHour = tm.Hour;
    currentMinute = tm.Minute;
    currentSecond = tm.Second;
  }
}

void printTime() {
  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tm.Year);
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
}

void setTime() {
  tm.Day = currentDate;
  tm.Month = currentMonth;
  tm.Year = currentYear;
  tm.Hour = currentHour;
  tm.Minute = currentMinute;
  tm.Second = currentSecond;
  RTC.write(tm);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
