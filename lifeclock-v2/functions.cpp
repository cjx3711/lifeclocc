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

bool userModifyVariable(bool &var) {
  if ((buttonPress(BTN_UP) || (longPressMills[BTN_UP] > SHORT_PRESS_TIMEOUT && repeatPhaseChange)) ||
      (buttonPress(BTN_DOWN)|| (longPressMills[BTN_DOWN] > SHORT_PRESS_TIMEOUT && repeatPhaseChange))) {
    var = !var;
    return true;
  }
  return false;
}

// ====================== DISPLAY HELPERS =======================


void numberToDisplay(unsigned long number, uint8_t decimal) {
  unsigned long workingCounter = number;

  uint8_t digit = workingCounter % 10;
  workingCounter = workingCounter / 10;

  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  // Send out the number after the decimal first.
  if (showDecisecond)
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[decimal]);
  else
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, ROTATION_CODE[(currentMills / 166) % 6]);
  
  // Send the number with the decimal digiz
  if (showDecisecond)
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
  else
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  for ( int i = 0; i < 9; i++ ) {
    digit = workingCounter % 10;
    workingCounter = workingCounter / 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}


void threeNumbersToDisplay(unsigned long weeks, unsigned long days, unsigned long seconds, uint8_t decimal) {
  unsigned long workingCounter = seconds;

  uint8_t digit = workingCounter % 10;
  workingCounter = workingCounter / 10;

  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  if (loopInt % 2 == 0 ) {
    // Send out the number after the decimal first
    if (showDecisecond) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[decimal]);
    } else {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, ROTATION_CODE[(currentMills / 166) % 6]);
    }
  
    // Send the number with the decimal digit
  
    if (showDecisecond)
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    else
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  
    // Send the rest of the seconds' digit
    for ( int i = 0; i < 4; i++ ) {
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }

    for ( int i = 0; i < 5; i++ ) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  } else {
    for ( int i = 0; i < 6; i++ ) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));

    workingCounter = days;
    digit = workingCounter % 10;

    // Show day
    if ( seconds % 2 ) { 
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    } else {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }

    workingCounter = weeks;
    // Show weeks
    uint8_t restOfNumbers = 4; // Used to make decimal point blink.
    if ( seconds % 2 ) { 
      // Send the number with the decimal digit
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
      restOfNumbers = 3;
    }
    
    // Send the rest of the days' digits
    for ( int i = 0; i < restOfNumbers; i++ ) {
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }
  }
  

  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void twoNumbersToDisplay(unsigned long days, unsigned long seconds, uint8_t decimal) {
  unsigned long workingCounter = seconds;

  uint8_t digit = workingCounter % 10;
  workingCounter = workingCounter / 10;

  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  if (DISPLAY_MODE == WEEKS) {
    
    if (loopInt % 2 == 0 ) {
      // Send out the number after the decimal first
      if (showDecisecond) {
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[decimal]);
      } else {
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, ROTATION_CODE[(currentMills / 166) % 6]);
      }
    
      // Send the number with the decimal digit
      if (showDecisecond)
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
      else
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    
      // Send the rest of the seconds' digit
      for ( int i = 0; i < 4; i++ ) {
        digit = workingCounter % 10;
        workingCounter = workingCounter / 10;
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
      }

      for ( int i = 0; i < 5; i++ ) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      for ( int i = 0; i < 6; i++ ) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));

      workingCounter = days;

      uint8_t restOfNumbers = 5; // Used to make decimal point blink.
      if ( seconds % 2 ) { 
        // Send the number with the decimal digit
        digit = workingCounter % 10;
        workingCounter = workingCounter / 10;
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
        restOfNumbers = 4;
      }
      
      // Send the rest of the days' digits
      for ( int i = 0; i < restOfNumbers; i++ ) {
        digit = workingCounter % 10;
        workingCounter = workingCounter / 10;
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
      }
    }
  } else {
    // Send out the number after the decimal first
    if (showDecisecond) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[decimal]);
    } else {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, ROTATION_CODE[(currentMills / 166) % 6]);
    }
  
    // Send the number with the decimal digit
    if (showDecisecond)
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    else
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  
    // Send the rest of the seconds' digit
    for ( int i = 0; i < 4; i++ ) {
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }

    workingCounter = days;

    uint8_t restOfNumbers = 5; // Used to make decimal point blink.
    if ( seconds % 2 ) { 
      // Send the number with the decimal digit
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
      restOfNumbers = 4;
    }
    
    // Send the rest of the days' digits
    for ( int i = 0; i < restOfNumbers; i++ ) {
      digit = workingCounter % 10;
      workingCounter = workingCounter / 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }
  }
  

  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void decisecondSettingToDisplay(bool on) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  if (on) {
    uint8_t digit = (millis() / 100) % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DOT));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    if (blinkPhase) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(N));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(O));
    }
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  } else {
    uint8_t anim = (millis() / 166) % 6;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, ROTATION_CODE[anim]);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    if (blinkPhase) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(F));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(F));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(O));
    }
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  }
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}
void timeToDisplay(uint16_t h, uint16_t m, uint16_t s, uint8_t blinkWhich) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  uint8_t digit;


  // Second
  if (blinkWhich == 3 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  } else {
    digit = s % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    digit = s / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }

  // Minute
  if (blinkWhich == 2 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  } else {
    digit = m % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    digit = m / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }


  // Hour
  if (blinkWhich == 1 && blinkPhase) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  } else {
    digit = h % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE_DP[digit]);
    digit = h / 10 % 10;
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
  }
  
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));


  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void dateToDisplay(uint16_t y, uint16_t m, uint16_t d, uint8_t blinkWhich) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  uint8_t digit;

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  if (loopInt % 2 == 0) {
    // Day
    if (blinkWhich == 3 && blinkPhase) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      digit = d % 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
      digit = d / 10 % 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }

    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));

    // Month
    if (blinkWhich == 2 && blinkPhase) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      digit = m % 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
      digit = m / 10 % 10;
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, NUMBER_CODE[digit]);
    }
  } else {
    for (int i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));

  }
  if (loopInt % 2 == 1) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
    // Year
    if (blinkWhich == 1 && blinkPhase) {
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
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
  } else {
    for (int i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  }


  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void lineToDisplay(unsigned long counter) {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  uint8_t pos = counter % 11;
  for (int i = 0 ; i < pos; i++) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
  }
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));

  for (int i = 0 ; i < 11 - pos - 1; i++) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
  }

  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}
  
  
void splashScreen() {

  uint16_t startTime = millis();
  bool left = true;
  while (millis() - startTime < 2000) {
    left = !left;
    digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
    if (DISPLAY_MODE == WEEKS) {
      if (left) {
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(O_DP));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(L));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        for (uint8_t i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      } else {
        for (uint8_t i = 0; i < 6; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(E));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(F));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(I));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(L));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
      }
    } else {
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(O_DP));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(L));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(E));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(F));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(I));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(L));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(DASH));
    }
    
    digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
  }

}

void testScreen() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  if (DISPLAY_MODE == WEEKS) {
    if (loopInt % 2) {
      for (uint8_t i = 0; i < 6; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(FULL));
      for (uint8_t i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      for (uint8_t i = 0; i < 6; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      for (uint8_t i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(FULL));
    }
  } else {
    for (uint8_t i = 0; i < 11; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(FULL));
  }
  
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void timedTestScreen() {
  uint16_t startTime = millis();
  bool left = true;
  while (millis() - startTime < 1000) {
    left = !left;
    digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
    if (left) {
      for (uint8_t i = 0; i < 6; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(FULL));
      for (uint8_t i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
    } else {
      for (uint8_t i = 0; i < 6; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
      for (uint8_t i = 0; i < 5; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(FULL));
    }
    digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
  }
}

void blankScreen() {
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers
  for ( uint8_t i = 0; i < 11; i++) shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

// ======================== DATE HELPERS =========================

bool validDate(SDate date) {
  if (date.date < 1 || date.date > 31) return false;
  if (date.month < 1 || date.month > 12) return false;
  if (date.year < 1900 || date.year > 2100) return false;
  return true;
}

uint16_t daysInMonth(uint16_t month, uint16_t year) {
  bool leapYear = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
  uint16_t months [12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if (leapYear && month == 2) {
    return 29;
  } else {
    return months[month-1];
  }
}

void getTime() {
  if (RTC.read(tm)) {
    currentDate.date = tm.Day;
    currentDate.month = tm.Month;
    currentDate.year = tm.Year;
    currentTime.hour = tm.Hour;
    currentTime.minute = tm.Minute;
    currentTime.second = tm.Second;
    // printTime(tm);
  } else {
    Serial.print("Error: Time could not be read");
  }
}

void setTime() {
  tm.Day = currentDate.date;
  tm.Month = currentDate.month;
  tm.Year = currentDate.year;
  tm.Hour = currentTime.hour;
  tm.Minute = currentTime.minute;
  tm.Second = currentTime.second;
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


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void printTime(tmElements_t time) {
  if (!DEBUG) return;

  Serial.print("Time = ");
  print2digits(currentTime.hour);
  Serial.write(':');
  print2digits(currentTime.minute);
  Serial.write(':');
  print2digits(currentTime.second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(currentDate.date);
  Serial.write('/');
  Serial.print(currentDate.month);
  Serial.write('/');
  Serial.print(currentDate.year);
  Serial.println();

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

void setupBlink(uint8_t whichLED, uint16_t onTime, uint16_t offTime) {
  if (!whichLED || whichLED == LED_PIN) digitalWrite(LED_PIN, HIGH);
  if (!whichLED || whichLED == LED_BDAY_PIN) digitalWrite(LED_BDAY_PIN, HIGH);
  if (!whichLED || whichLED == LED_CLOCK_PIN) digitalWrite(LED_CLOCK_PIN, HIGH);
  if (!whichLED || whichLED == LED_TIME_PIN) digitalWrite(LED_TIME_PIN, HIGH);
  if (!whichLED || whichLED == LED_DATE_PIN) digitalWrite(LED_DATE_PIN, HIGH);
  delay(onTime);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_BDAY_PIN, LOW);
  digitalWrite(LED_CLOCK_PIN, LOW);
  digitalWrite(LED_TIME_PIN, LOW);
  digitalWrite(LED_DATE_PIN, LOW);
  delay(offTime);
}

void setupBlink() {
  setupBlink(false, 150, 100);
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
    Serial.println("Debug Mode");
    changeState(STATE_DEBUG);
  }

  if (longPressMills[BTN_UP] > LONG_PRESS_TIMEOUT && longPressMills[BTN_DOWN] > LONG_PRESS_TIMEOUT) {
    Serial.println("Game Mode");
    changeState(STATE_GAME);
  }

  if (longPressMills[BTN_BDAY] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_BIRTHDAY);
    programSubState = 1;
  }

  if (longPressMills[BTN_CLOCK] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_SET_CLOCK);
  }

  if (anyButtonRelease()) timeoutMills = 0; // Reset the timeout counter
  if (timeoutMills > CLOCK_STATE_TIMEOUT) {
    programSubState = 0;
  }

  if (buttonRelease(BTN_TIME)) {
    if (programSubState != 2) programSubState = 2;
    else programSubState = 0;
  }

  if (buttonRelease(BTN_DATE)) {
    if (programSubState != 1) programSubState = 1;
    else programSubState = 0;
  }

  counter = getSecondsTillDeath();
  switch(programSubState) {
    case 0: // Regular Clock Mode
      if (DISPLAY_MODE == WEEKS)        
        threeNumbersToDisplay((counter / SECONDS_IN_DAY) / DAYS_IN_WEEK, (counter / SECONDS_IN_DAY) % DAYS_IN_WEEK, counter % SECONDS_IN_DAY, 9 - (secondsToSubtract / 100));
      else if (DISPLAY_MODE == DAYS)
        twoNumbersToDisplay(counter / SECONDS_IN_DAY, counter % SECONDS_IN_DAY, 9 - (secondsToSubtract / 100));
      else if (DISPLAY_MODE == SECONDS)
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

  uint16_t maxDays = daysInMonth(currentDate.month, currentDate.year + 1970);
    switch(programSubState) {
    case 0: // To allow for the first button release
    case 1: // Year
      timeChanged = userModifyVariable(currentDate.year, 30, 150); break;
    case 2: // Month
      timeChanged = userModifyVariable(currentDate.month, 1, 12); break;
    case 3: // Day
      timeChanged = userModifyVariable(currentDate.date, 1, maxDays); break;
    case 4: // Hour
      timeChanged = userModifyVariable(currentTime.hour, 0, 23); break;
    case 5: // Minute
      timeChanged = userModifyVariable(currentTime.minute, 0, 59); break;
    case 6: // Second
      timeChanged = userModifyVariable(currentTime.second, 0, 59); break;
    case 7: // Decisecond Display
      userModifyVariable(showDecisecond); break;
  }
  maxDays = daysInMonth(currentDate.month, currentDate.year + 1970);
  if (currentDate.date > maxDays) currentDate.date = maxDays;

  if (timeChanged) {
    setTime();
    Serial.print("Time Changed: ");
    printTime(tm);
  }

  if (anyButtonRelease()) timeoutMills = 0;
  if (timeoutMills > SET_STATE_TIMEOUT) changeState(STATE_CLOCK);

  if ( buttonRelease(BTN_NEXT) ) {
    programSubState++;
    if (programSubState > 7) programSubState = 1;
  }

  if ( buttonRelease(BTN_PREV) ) {
    programSubState--;
    if (programSubState < 1) programSubState = 7;
  }
  
  if (longPressMills[BTN_NEXT] > LONG_PRESS_TIMEOUT || longPressMills[BTN_PREV] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }


  if (programSubState <= 3) {
    dateToDisplay(tm.Year + 1970, tm.Month, tm.Day, programSubState == 0 ? 1 : programSubState);
  } else if (programSubState <= 6) {
    timeToDisplay(tm.Hour, tm.Minute, tm.Second, programSubState - 3);
  } else {
    decisecondSettingToDisplay(showDecisecond);
  }
}
void stateSetBirthday() {
  digitalWrite(LED_BDAY_PIN, HIGH);
  
  bool timeChanged = false;
  uint16_t maxDays = daysInMonth(birthDate.month, birthDate.year);
  switch(programSubState) {
    case 0: // Year
      timeChanged = userModifyVariable(birthDate.year, 1900, 2100); break;
    case 1: // Month
      timeChanged = userModifyVariable(birthDate.month, 1, 12); break;
    case 2: // Day
      timeChanged = userModifyVariable(birthDate.date, 1, maxDays); break;
  }

  maxDays = daysInMonth(birthDate.month, birthDate.year);
  if (birthDate.date > maxDays) birthDate.date = maxDays;
  if (timeChanged) eeprom_write_block((const void*)&birthDate, (void*)0, sizeof(birthDate));

  dateToDisplay(birthDate.year, birthDate.month, birthDate.date, programSubState + 1);

  if ( buttonRelease(BTN_NEXT) ) {
    if ( programSubState == 2 ) programSubState = 0;
    else programSubState++;
  }

  if ( buttonRelease(BTN_PREV) ) {
    if ( programSubState == 0 ) programSubState = 2;
    else programSubState--;
  }

  if (longPressMills[BTN_BDAY] > LONG_PRESS_TIMEOUT || longPressMills[BTN_CLOCK] > LONG_PRESS_TIMEOUT) {
    changeState(STATE_CLOCK);
  }

  if (anyButtonRelease()) timeoutMills = 0;
  if (timeoutMills > SET_STATE_TIMEOUT) changeState(STATE_CLOCK);

}
void stateDebug() {
  if (blinkPhase) {
    testScreen();
    digitalWrite(LED_DATE_PIN, HIGH);
    digitalWrite(LED_CLOCK_PIN, HIGH);
    digitalWrite(LED_TIME_PIN, HIGH);
    digitalWrite(LED_BDAY_PIN, HIGH);
  } else {
    blankScreen();
    digitalWrite(LED_DATE_PIN, LOW);
    digitalWrite(LED_CLOCK_PIN, LOW);
    digitalWrite(LED_TIME_PIN, LOW);
    digitalWrite(LED_BDAY_PIN, LOW);
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


// =================== GAME STATE ====================

void initCharacter() {
  character.blinkTimer = 0;
  character.stateTimer = 0;
  character.moveTimer = 0;
  character.isBlink = false;
  character.pos = 3;
  character.charState = random(0,3);
}

void calcAndRenderCharacter(SCharacter& c) {
  // Calculations here
  c.blinkTimer -= millsDelta;
  c.stateTimer -= millsDelta;
  c.moveTimer -= millsDelta;
  if ( c.moveTimer <= 0 ) {
    c.moveTimer = random(500, 1700);
    if ( random() % 2 ) {
      if ( c.pos > 0 ) c.pos--;
    } else {
      if ( c.pos < 7 ) c.pos++;
    }
  }

  if ( c.blinkTimer <= 0 ) {
    c.isBlink = !c.isBlink;
    if ( c.isBlink ) {
      c.blinkTimer = random(50, 200);
    } else {
      c.blinkTimer = random(800, 5000);
    }
  }
  if ( c.stateTimer <= 0 ) {
    c.stateTimer = random(800, 5000);
    c.charState = random(0,100);
    if ( c.charState < 60 ) c.charState = CHAR_FORWARD;
    else if ( c.charState < 80 ) c.charState = CHAR_LEFT;
    else c.charState = CHAR_RIGHT;
  }

  // Rendering here
  digitalWrite(SR_LATCH_PIN, LOW); // Freezes the shift registers

  for ( int i = 0; i < 11; i++ ) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  }

  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_RIGHT));
  if ( c.isBlink ) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_DASH));
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_DASH));
  } else {
    switch (c.charState) {
      case CHAR_FORWARD:
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_LEFT_C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_RIGHT_C));
      break;
      case CHAR_LEFT:
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_LEFT_C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_LEFT_C));
      break;
      case CHAR_RIGHT:
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_RIGHT_C));
        shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_RIGHT_C));
      break;
    }

  }
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(LOWER_LEFT));


  for ( int i = 0; i < c.pos; i++ ) {
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, DSP(BLANK));
  }
  digitalWrite(SR_LATCH_PIN, HIGH); // Unfreezes the shift registers
}

void stateGame() {
  calcAndRenderCharacter(character);


  if (anyButtonPress()) {
    changeState(STATE_CLOCK);
  }
}
