#include "functions.h"

bool validDate(SDate date) {
  if (date.date < 1 || date.date > 31) return false;
  if (date.month < 1 || date.month > 12) return false;
  if (date.year < 1900 || date.year > 2100) return false;
  return true;
}


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

uint16_t daysInMonth(uint16_t month, uint16_t year) {
  bool leapYear = year % 4 == 0;
  uint16_t months [12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if (leapYear && month == 2) {
    return 29;
  } else {
    return months[month-1];
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

long getSecondsTillDeath() {
  tm2.Day = birthDate.date;
  tm2.Month = birthDate.month;
  tm2.Year = birthDate.year + 80 - 1970;
  tm2.Hour = tm2.Minute = tm2.Second = 0;
  time_t t1 = makeTime(tm);
  time_t t2 = makeTime(tm2);
  return t2 - t1;
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
  tm.Day = currentDate.date;
  tm.Month = currentDate.month;
  tm.Year = currentDate.year;
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
