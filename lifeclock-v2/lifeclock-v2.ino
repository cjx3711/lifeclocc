int number = 0;

//Pin connected to ST_CP of 74HC595
int latchPin = 7;
//Pin connected to SH_CP of 74HC595
int clockPin = 6;
////Pin connected to DS of 74HC595
int dataPin = 8;

int potPin = 0;



uint8_t numbers[] = {
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B10111110,
  B11100000,
  B11111110,
  B11110110,
};

void setup() {

   //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Invert all the bits. This is needed if we are using common cathode.
  for (int i = 0; i < 10; i++) { 
    numbers[i] = numbers[i] ^ B11111111;
  }
}




void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs

  // pause before next value:

  int brightness = analogRead(potPin) * (230.0f / 1024.0f) + 26;
  analogWrite(5, brightness);

  int val = analogRead(potPin) * (1000.0f / 1024.0f);
  
  digitalWrite(latchPin, LOW);
  for ( int i = 0; i < 3; i++ ) {
    uint8_t digit = val % 10;
    shiftOut(dataPin, clockPin, LSBFIRST, numbers[digit]);
    val = val / 10;
  }
  digitalWrite(latchPin, HIGH);

  delay(100);


  
}
