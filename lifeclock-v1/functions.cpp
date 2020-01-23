#include "functions.h"


// =============================== INPUT HELPERS ============================

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
  if (buttonPress(BTN_UP) || (longPressMills[BTN_UP] > SHORT_PRESS_TIMEOUT && repeatPhaseChange)) {
    if (var == max) var = min;
    else var++;
    return true;
  } else if(buttonPress(BTN_DOWN)|| (longPressMills[BTN_DOWN] > SHORT_PRESS_TIMEOUT && repeatPhaseChange)) {
    if (var == min) var = max;
    else var--;
    return true;
  }
  return false;
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

// =============================== DISPLAY HELPERS ============================
void numberToDisplay(unsigned long number) {
  unsigned long workingCounter = number;
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
  digitalWrite(FIRST_PIN + 9 - which, LOW);
  writeDigitToBus(digit);
  digitalWrite(FIRST_PIN + 9 - which, HIGH);
}

void setBlank(uint8_t which) {
  digitalWrite(FIRST_PIN + 9 - which, LOW);
  writeBlankToBus();
  digitalWrite(FIRST_PIN + 9 - which, HIGH);
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

void blankScreen() {
  for (int i = 0; i < 10; i++) digitalWrite(FIRST_PIN + i, LOW);
  writeBlankToBus();
  for (int i = 0; i < 10; i++) digitalWrite(FIRST_PIN + i, HIGH);
}

void splashScreen() {
  // Splash screen is just the debug screen
  debugScreen();
}

void debugScreen() {
  for (int i = 0; i < 10; i++) digitalWrite(FIRST_PIN + i, LOW);
  writeDigitToBus(8);
  for (int i = 0; i < 10; i++) digitalWrite(FIRST_PIN + i, HIGH);
}


// =============================== DATE HELPERS ============================

bool validDate(SDate date) {
  if (date.date < 1 || date.date > 31) return false;
  if (date.month < 1 || date.month > 12) return false;
  if (date.year < 1900 || date.year > 2100) return false;
  return true;
}

uint16_t daysInMonth(uint16_t month, uint16_t year) {
  bool leapYear = year % 4 == 0;
  uint16_t months [12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if (leapYear && month == 2) {
    return 29;
  } else {
    return months[month-1];
  }
}

void printTime() {
  if (!DEBUG) return;
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

void getTime() {
  if (RTC.read(tm)) {
    currentDate.date = tm.Day;
    currentDate.month = tm.Month;
    currentDate.year = tm.Year;
    currentHour = tm.Hour;
    currentMinute = tm.Minute;
    currentSecond = tm.Second;
  }
}

void setTime() {
  tm.Day = currentDate.date;
  tm.Month = currentDate.month;
  tm.Year = currentDate.year;
  tm.Hour = currentHour;
  tm.Minute = currentMinute;
  tm.Second = currentSecond;
  RTC.write(tm);
}

unsigned long getSecondsTillDeath() {
  tm2.Day = birthDate.date;
  tm2.Month = birthDate.month;
  tm2.Year = birthDate.year + 80 - 1970;
  tm2.Hour = tm2.Minute = tm2.Second = 0;
  time_t t1 = makeTime(tm);
  time_t t2 = makeTime(tm2);
  // printTime(tm);
  // printTime(tm2);
  return t2 - t1;
}

// ================================= SETUP =====================================
void setupBlink() {
  digitalWrite(LED_PIN, HIGH);
  analogWrite(SET_LED_PIN, 128);
  analogWrite(DATE_LED_PIN, 128);
  analogWrite(TIME_LED_PIN, 128);
  analogWrite(BIRTHDAY_LED_PIN, 128);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  analogWrite(SET_LED_PIN, 0);
  analogWrite(DATE_LED_PIN, 0);
  analogWrite(TIME_LED_PIN, 0);
  analogWrite(BIRTHDAY_LED_PIN, 0);
  delay(300);
}


void initVariables() {
  longPressMills[BTN_RESET] = 0; // Used for timing long presses
  longPressMills[BTN_UP] = 0; // Used for quickly setting numbers
  longPressMills[BTN_DOWN] = 0; // Used for quickly setting numbers
  timeoutMills = 0;

  currentMinute = 0;
  currentHour = 0;
  currentSecond = 0;

  programState = 0;
  programSubState = 0;

  blinkPhase = prevBlinkPhase = blinkPhaseChange = false;
  repeatPhase = prevRepeatPhase = repeatPhaseChange = false;
}

void initPins() {
  // Initialise the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  pinMode(SET_LED_PIN, OUTPUT);
  pinMode(DATE_LED_PIN, OUTPUT);
  pinMode(TIME_LED_PIN, OUTPUT);
  pinMode(BIRTHDAY_LED_PIN, OUTPUT);

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
  for (int i = 0; i < 10; i++) pinMode(FIRST_PIN + i, OUTPUT);
}

// Read or initialise birthday
void readBirthday() {
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
}


// ================================ STATES =====================================

void preLoop() {
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
  if (blinkPhaseChange) {
    if (DEBUG) {
      Serial.println(counter);
      Serial.println(buttonStates[0]);
      Serial.println(buttonStates[1]);
      Serial.println(buttonStates[2]);
    }
  }

  // Reset all the indicatr lights
  digitalWrite(LED_PIN, LOW);
  analogWrite(SET_LED_PIN, 0);
  analogWrite(DATE_LED_PIN, 0);
  analogWrite(TIME_LED_PIN, 0);
  analogWrite(BIRTHDAY_LED_PIN, 0);
}

void buttonStatePreLoop() {
  buttonStates[BTN_UP] = digitalRead(BTN_UP_PIN);
  buttonStates[BTN_DOWN] = digitalRead(BTN_DOWN_PIN);
  buttonStates[BTN_RESET] = digitalRead(BTN_RESET_PIN);

  // Global (not within states) input handlers
  for (int i = 0; i < 3; i++) {
    if (buttonStates[i]) {
      digitalWrite(LED_PIN, HIGH);
      longPressMills[i] += millsDelta;
    } else {
      longPressMills[i] = 0;
    }
  }
}

void buttonStatePostLoop() {
  for ( int i = 0; i < 3; i++ ) buttonStatesPrev[i] = buttonStates[i];
}

void changeState(uint8_t state) {
  programState = state;
  longPressMills[BTN_RESET] = 0; // Used for timing long presses
  longPressMills[BTN_UP] = 0; // Used for quickly setting numbers
  longPressMills[BTN_DOWN] = 0; // Used for quickly setting numbers
  programSubState = 0; // Used for the state within the state
  timeoutMills = 0; // Used for timeouts and reverting to base state
}

void stateClock() {
  if (longPressMills[BTN_UP] > SHORT_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > SHORT_PRESS_TIMEOUT && longPressMills[BTN_RESET] > SHORT_PRESS_TIMEOUT) {
    changeState(STATE_DEBUG);
  }

  // Input handlers
  if (longPressMills[BTN_RESET] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_CLOCK);
  }

  if (longPressMills[BTN_UP] > LONG_PRESS_TIMEOUT || longPressMills[BTN_DOWN] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_BIRTHDAY);
  }

  if (buttonRelease(BTN_UP)) {
    if (programSubState != 2) programSubState = 2;
    else programSubState = 0;
  }

  if (buttonRelease(BTN_DOWN)) {
    if (programSubState != 1) programSubState = 1;
    else programSubState = 0;
  }

  if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
  if (timeoutMills > CLOCK_STATE_TIMEOUT) {
    programSubState = 0;
  }

  // Display Handers
  counter = getSecondsTillDeath();
  switch(programSubState) {
    case 0:
      numberToDisplay(counter); break;
    case 1:
      analogWrite(DATE_LED_PIN, 128);
      dateToDisplay(currentDate.date, currentDate.month, currentDate.year + 1970, 0); break;
    case 2:
      analogWrite(TIME_LED_PIN, 128);
      timeToDsplay(currentHour, currentMinute, currentSecond, 0); break;
  }
}

void stateDebug() {
    debugScreen();
    digitalWrite(LED_PIN, HIGH);
    analogWrite(SET_LED_PIN, 128);
    analogWrite(DATE_LED_PIN, 128);
    analogWrite(TIME_LED_PIN, 128);
    analogWrite(BIRTHDAY_LED_PIN, 128);

    // Input Handlers
    if (anyButtonRelease()) {
      changeState(STATE_CLOCK);
    }
}
// Note: Birth year is stored as the actual year. e.g. 1995
//       Clock year is stored as years since 1970. e.g. 25
void stateSetClock() {
  analogWrite(SET_LED_PIN, 128);

  // Input Handlers
  bool timeChanged = false;
  uint16_t maxDays = daysInMonth(currentDate.month, currentDate.year + 1970);
  switch(programSubState) {
    case 0: // To allow for the first button release
    case 1: // Day
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

  if (timeChanged) {
    setTime();
    printTime();
  }

  if ( buttonRelease(BTN_RESET) ) {
    programSubState++;
    if (programSubState > 6) programSubState = 1;
  }

  if (longPressMills[BTN_RESET] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
  if (timeoutMills > SET_STATE_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  if (programSubState <= 3) {
    if ( blinkPhase ) analogWrite(DATE_LED_PIN, 128);
    dateToDisplay(currentDate.date, currentDate.month, currentDate.year + 1970, programSubState == 0 ? 1 : programSubState);
  } else {
    if ( blinkPhase ) analogWrite(TIME_LED_PIN, 128);
    timeToDsplay(currentHour, currentMinute, currentSecond, programSubState - 3);
  }
}

// Note: Birth year is stored as the actual year. e.g. 1995
//       Clock year is stored as years since 1970. e.g. 25
void stateSetBirthday() {
  analogWrite(SET_LED_PIN, 128);
  analogWrite(BIRTHDAY_LED_PIN, 128);
  bool timeChanged = false;
  uint16_t maxDays = daysInMonth(birthDate.month, birthDate.year);
  switch(programSubState) {
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

  dateToDisplay(birthDate.date, birthDate.month, birthDate.year, programSubState + 1);

  if ( buttonRelease(BTN_RESET) ) {
    programSubState++;
    if (programSubState >= 3) programSubState = 0;
  }

  if (longPressMills[BTN_RESET] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
  if (timeoutMills > SET_STATE_TIMEOUT) {
    changeState(STATE_CLOCK);
  }
}
