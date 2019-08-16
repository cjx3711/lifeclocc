#define LED_PIN 6
#define BTN_UP_PIN 7
#define BTN_DOWN_PIN 5
#define BTN_RESET_PIN 4

#define BCDA_PIN 23
#define BCDB_PIN 24
#define BCDC_PIN 25
#define BCDD_PIN 26

#define FIRST_PIN 8

#define SECONDS_IN_DAY 31536000
#define LONG_PRESS_TIMEOUT 3000
#define SET_STATE_TIMEOUT 10000
#define BLINK_MS 500

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
unsigned long setStateTimeout = 0;
bool blinkPhase;


// Date calculation workings
uint16_t currentDate = 24;
uint16_t currentMonth = 7;
uint16_t currentYear = 2019;

bool stateSet = false;
uint8_t stateSetState = 0;

void numberToDisplay(long number) {
  long workingCounter = number;
  for ( int i = 0; i < 10; i++ ) {
    digit = workingCounter % 10;
    workingCounter = workingCounter / 10;
    if ( workingCounter == 0 && digit == 0 ) setBlank(i);
    else setDigit(i, digit);
  }
}

void dateToDisplay(uint16_t d, uint16_t m, uint16_t y, uint8_t state) {
  uint16_t workings;
  // Set date
  workings = d;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( state == 0 && blinkPhase ) setBlank(i + 8);
    else setDigit(i + 8, digit);
  }

  // Set month
  workings = m;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( state == 1 && blinkPhase ) setBlank(i + 5);
    else setDigit(i + 5, digit);
  }

  // Set year
  workings = y;
  for ( int i = 0; i < 4; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    if ( state == 2 && blinkPhase ) setBlank(i);
    else setDigit(i, digit);
  }

  // Blank the rest of the display
  digitalWrite(FIRST_PIN + 7, LOW);
  digitalWrite(FIRST_PIN + 4, LOW);  
  writeBlankToBus();
  digitalWrite(FIRST_PIN + 7, HIGH);
  digitalWrite(FIRST_PIN + 4, HIGH); 
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
  return buttonRelease[BTN_UP] || buttonRelease[BTN_DOWN] || buttonRelease[BTN_RESET]
}
bool buttonRelease(uint8_t btn) {
  return !buttonStates[btn] && buttonStatesPrev[btn]
}

bool buttonPress(uint8_t btn) {
  return buttonStates[btn] && !buttonStatesPrev[btn]
}


void userModifyVariable(uint16_t &var, uint16_t min, uint16_t max) {
  if (buttonRelease(BTN_UP)) {
    if (var == max) var = min;
    else var++;
  } else if(buttonRelease(BTN_DOWN)) {
    if (var == min) var = max;
    else var--;
  }
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
  numberToDisplay(counter);
}

void loop() {
  currentMills = millis();
  millsDelta = currentMills - prevMills;
  prevMills = currentMills;
  outstandingMills += millsDelta;
  setStateTimeout += millsDelta;
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

  if ( !stateSet ) {
    // ----- REGULAR CLOCK STATE -------
    if (buttonStates[BTN_RESET]) {
      digitalWrite(LED_PIN, HIGH);
      longPressMills += millsDelta;
    } else {
      longPressMills = 0;
    }

    if (longPressMills > LONG_PRESS_TIMEOUT) {
      stateSet = true;
      longPressMills = 0;
      stateSetState = 0;
      setStateTimeout = 0;
    }

    numberToDisplay(counter);
  } else {
    // ----- SETTING STATE -----
    
    swtich(stateSetState) {
      case 0: // Day
        userModifyVariable(currentDate, 1, 31); break;
      case 1: // Month
        userModifyVariable(currentMonth, 1, 12); break;
      case 2: // Year
        userModifyVariable(currentYear, 1900, 2100); break;
    }
    
    if ( buttonRelease(BTN_RESET) ) {
      if (stateSetState < 2) {
        stateSetState++;
      } else {
        stateSet = false;
      }
    }
    
    if (anyButtonRelease()) setStateTimeout = 0; // Reset the timeout counter
    if (setStateTimeout > SET_STATE_TIMEOUT) {
      stateSet = false;
    }
    
    dateToDisplay(currentDate, currentMonth, currentYear, stateSetState);
  }


  for ( int i = 0; i < 3; i++ ) {
    buttonStatesPrev[i] = buttonStates[i];
  }

  delay(10);
}
