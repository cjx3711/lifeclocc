int LED_PIN = 6;
int BTN_UP_PIN = 7;
int BTN_DOWN_PIN = 5;
int BTN_RESET_PIN = 4;

int BCDA_PIN = 23;
int BCDB_PIN = 24;
int BCDC_PIN = 25;
int BCDD_PIN = 26;

int FIRST_PIN = 8;

uint8_t digit = 3;

#define SECONDS_IN_DAY 31536000


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
bool blinkPhase;


uint8_t currentDate = 24;
uint8_t currentMonth = 7;
uint16_t currentYear = 2019;

bool stateSet = false;
uint8_t stateSetState = 0;


void numberToDisplay(long number) {
  long workingCounter = number;
  for ( int i = 0; i < 10; i++ ) {
    digit = workingCounter % 10;
    workingCounter = workingCounter / 10;
    digitalWrite(FIRST_PIN + i, LOW); 
    if (workingCounter == 0 && digit == 0) writeBlankToBus();
    else writeDigitToBus(digit);
    digitalWrite(FIRST_PIN + i, HIGH);
  }
}

void dateToDisplay(uint8_t d, uint8_t m, uint16_t y, uint8_t state) {
  uint16_t workings;
  // Set date
  workings = d;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    digitalWrite(FIRST_PIN + 8 + i, LOW); 
    if ( state == 0 && blinkPhase ) writeBlankToBus();
    else writeDigitToBus(digit);
    digitalWrite(FIRST_PIN + 8 + i, HIGH);
  }

  // Set month
  workings = m;
  for ( int i = 0; i < 2; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    digitalWrite(FIRST_PIN + 5 + i, LOW); 
    if ( state == 1 && blinkPhase ) writeBlankToBus();
    else writeDigitToBus(digit);
    digitalWrite(FIRST_PIN + 5 + i, HIGH);
  }

  // Set year
  workings = y;
  for ( int i = 0; i < 4; i++ ) {
    digit = workings % 10;
    workings = workings / 10;
    digitalWrite(FIRST_PIN + i, LOW); 
    if ( state == 2 && blinkPhase ) writeBlankToBus();
    else writeDigitToBus(digit);
    digitalWrite(FIRST_PIN + i, HIGH);
  }

  // Blank the rest of the display
  digitalWrite(FIRST_PIN + 7, LOW);
  digitalWrite(FIRST_PIN + 4, LOW);  
  writeBlankToBus();
  digitalWrite(FIRST_PIN + 7, HIGH);
  digitalWrite(FIRST_PIN + 4, HIGH); 
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
void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the pushbutton pins as an input:
  pinMode(BTN_UP_PIN, INPUT);
  pinMode(BTN_DOWN_PIN, INPUT);
  pinMode(BTN_RESET_PIN, INPUT);

  pinMode(BCDA_PIN, OUTPUT);
  pinMode(BCDB_PIN, OUTPUT);
  pinMode(BCDC_PIN, OUTPUT);
  pinMode(BCDD_PIN, OUTPUT);

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
  blinkPhase = (currentMills / 500) % 2;
  

  while (outstandingMills > 1000) {
    outstandingMills -= 1000;
    counter--;
  }
  
  digitalWrite(LED_PIN, LOW); 
  buttonStates[0] = digitalRead(BTN_UP_PIN);
  buttonStates[1] = digitalRead(BTN_DOWN_PIN);
  buttonStates[2] = digitalRead(BTN_RESET_PIN);



//  // Detect if a button is clicked (state change from low to high)
//  if ( buttonStates[0] && !buttonStatesPrev[0]) {
//    digitalWrite(LED_PIN, HIGH);
//    counter++;
//  }
//
//  if ( buttonStates[1] && !buttonStatesPrev[1]) {
//    digitalWrite(LED_PIN, HIGH); 
//    counter--;
//  }
//
//  if ( buttonStates[2] && !buttonStatesPrev[2]) {
//    digitalWrite(LED_PIN, HIGH); 
//    stateSet = !stateSet;
//  }
  
  digitalWrite(LED_PIN, LOW);

  if ( !stateSet ) {
    if (buttonStates[2]) {
      digitalWrite(LED_PIN, HIGH);
      longPressMills += millsDelta;
    } else {
      longPressMills = 0;
    }

    if (longPressMills > 3000) {
      stateSet = true;
      longPressMills = 0;
      stateSetState = 0;
    }

    numberToDisplay(counter);
  } else {


    if ( buttonStates[2] && !buttonStatesPrev[2]) {
      if (stateSetState < 2) {
        stateSetState++;
      } else {
        stateSet = false;
      }
    }
    dateToDisplay(currentDate, currentMonth, currentYear, stateSetState);
  }


  for ( int i = 0; i < 3; i++ ) {
    buttonStatesPrev[i] = buttonStates[i];
  }

  delay(10);
}
