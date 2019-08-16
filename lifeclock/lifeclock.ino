int LED_PIN = 6;
int BTN_UP_PIN = 7;
int BTN_DOWN_PIN = 5;
int BTN_RESET_PIN = 4;

int BCDA_PIN = 23;
int BCDB_PIN = 24;
int BCDC_PIN = 25;
int BCDD_PIN = 26;

uint8_t digit = 3;



// Working variables

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [3] = {false, false, false};
bool buttonStates [3] = {false, false, false};

long counter = 0;
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
  
  Serial.begin(9600);

  digitalWrite(BCDA_PIN, (digit >> 0) & 1);
  digitalWrite(BCDB_PIN, (digit >> 1) & 1); 
  digitalWrite(BCDC_PIN, (digit >> 2) & 1); 
  digitalWrite(BCDD_PIN, (digit >> 3) & 1); 
  
  Serial.print((digit >> 0) & 1);
  Serial.print("\n");
  Serial.print((digit >> 1) & 1);
  Serial.print("\n");
  Serial.print((digit >> 2) & 1);
  Serial.print("\n");
  Serial.print((digit >> 3) & 1);
  Serial.print("\n");
}

void loop() {
  digitalWrite(LED_PIN, LOW); 
  buttonStates[0] = digitalRead(BTN_UP_PIN);
  buttonStates[1] = digitalRead(BTN_DOWN_PIN);
  buttonStates[2] = digitalRead(BTN_RESET_PIN);


  // Detect if a button is clicked (state change from low to high)
  if ( buttonStates[0] && !buttonStatesPrev[0]) {
    digitalWrite(LED_PIN, HIGH);
    if (digit == 9) digit = 0;
    else digit++;
  }

  if ( buttonStates[1] && !buttonStatesPrev[1]) {
    digitalWrite(LED_PIN, HIGH); 

    if (digit == 0) digit = 9;
    else digit--;

  }

  if ( buttonStates[2] && !buttonStatesPrev[2]) {
    digitalWrite(LED_PIN, HIGH); 

    digit = 0;

  }

  for ( int i = 0; i < 3; i++ ) {
    buttonStatesPrev[i] = buttonStates[i];
  }



  digitalWrite(BCDA_PIN, (digit >> 0) & 1);
  digitalWrite(BCDB_PIN, (digit >> 1) & 1); 
  digitalWrite(BCDC_PIN, (digit >> 2) & 1); 
  digitalWrite(BCDD_PIN, (digit >> 3) & 1); 


  delay(10);
}
