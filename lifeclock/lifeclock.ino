int LED_PIN = 6;
int BTN_UP_PIN = 7;
int BTN_DOWN_PIN = 5;
int BTN_RESET_PIN = 4;

// Working variables

// Button states for UP, DOWN, RESET
bool buttonStatesPrev [3] = {false, false, false};
bool buttonStates [3] = {false, false, false};

int counter = 0;
void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the pushbutton pins as an input:
  pinMode(BTN_UP_PIN, INPUT);
  pinMode(BTN_DOWN_PIN, INPUT);
  pinMode(BTN_RESET_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_PIN, LOW); 
  buttonStates[0] = digitalRead(BTN_UP_PIN);
  buttonStates[1] = digitalRead(BTN_DOWN_PIN);
  buttonStates[2] = digitalRead(BTN_RESET_PIN);


  // Detect if a button is clicked (state change from low to high)
  if ( buttonStates[0] && !buttonStatesPrev[0]) {
    digitalWrite(LED_PIN, HIGH);
    counter ++;
      Serial.print(counter);
  Serial.print("\n");
  }

  if ( buttonStates[1] && !buttonStatesPrev[1]) {
    digitalWrite(LED_PIN, HIGH); 

    counter --;
      Serial.print(counter);
  Serial.print("\n");
  }

  if ( buttonStates[2] && !buttonStatesPrev[2]) {
    digitalWrite(LED_PIN, HIGH); 

    counter = 0;
      Serial.print(counter);
  Serial.print("\n");
  }

  for ( int i = 0; i < 3; i++ ) {
    buttonStatesPrev[i] = buttonStates[i];
  }



  delay(10);
}
