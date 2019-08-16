int LED_PIN = 6;
int BTN_UP_PIN = 7;
int BTN_DOWN_PIN = 5;
int BTN_RESET_PIN = 4;

// Working variables

// Button states for UP, DOWN, RESET
bool buttonStates [3] = {false, false, false};

void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the pushbutton pins as an input:
  pinMode(BTN_UP_PIN, INPUT);
  pinMode(BTN_DOWN_PIN, INPUT);
  pinMode(BTN_RESET_PIN, INPUT);
}

void loop() {
  buttonStates[0] = digitalRead(BTN_UP_PIN);
  buttonStates[1] = digitalRead(BTN_DOWN_PIN);
  buttonStates[2] = digitalRead(BTN_RESET_PIN);

  if ( buttonStates[0] || buttonStates[1] || buttonStates[2] ) {
    digitalWrite(LED_PIN, HIGH); 
  } else {
    digitalWrite(LED_PIN, LOW); 
  }
}
