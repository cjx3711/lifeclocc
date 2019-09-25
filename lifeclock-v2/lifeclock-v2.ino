#include "functions.h"

unsigned long counter;
unsigned long millsDelta;
unsigned long prevMills;
unsigned long currentMills;
uint16_t secondsToSubtract;

uint8_t NUMBER_CODE[] = {
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

uint8_t NUMBER_CODE_DP[] = {
  B11111101,
  B01100001,
  B11011011,
  B11110011,
  B01100111,
  B10110111,
  B10111111,
  B11100001,
  B11111111,
  B11110111,
};

void setup() {
  initPins();
  testScreen();
  setupBlink();
  initVariables();
  setupBlink();
  delay(1000);
  splashScreen();
  Serial.begin(9600);
  setupBlink();
  delay(3000);


  prevMills = currentMills = millis();
}


void loop() {
  prevMills = currentMills;
  currentMills = millis();
  millsDelta = currentMills - prevMills;
  secondsToSubtract += millsDelta;

  if ( secondsToSubtract > 1000 ) {
    counter--;
    secondsToSubtract -= 1000;
  }

  analogWrite(BDAY_LED_PIN, 0);
  analogWrite(CLOCK_LED_PIN, 0);
  analogWrite(TIME_LED_PIN, 0);
  analogWrite(DATE_LED_PIN, 0);
  if (digitalRead(BDAY_BTN_PIN)) analogWrite(BDAY_LED_PIN, 255);
  if (digitalRead(CLOCK_BTN_PIN)) analogWrite(CLOCK_LED_PIN, 255);
  if (digitalRead(TIME_BTN_PIN)) analogWrite(TIME_LED_PIN, 255);
  if (digitalRead(DATE_BTN_PIN)) analogWrite(DATE_LED_PIN, 255);

  int brightness = analogRead(POTIOMETER_PIN) * ((255.0f - MIN_BRIGHTNESS) / 1024.0f) + MIN_BRIGHTNESS;
  analogWrite(DSP_POWER_PIN, brightness);
  numberToDisplay(counter, 9 - (secondsToSubtract / 100));
}
