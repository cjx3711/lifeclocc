#ifndef CONSTANTS_H
#define CONSTANTS_H

//Pin connected to ST_CP of 74HC595
#define SR_LATCH_PIN 8
//Pin connected to SH_CP of 74HC595
#define SR_CLOCK_PIN 7
////Pin connected to DS of 74HC595
#define SR_DATA_PIN 9

#define DSP_POWER_PIN 6
#define POTIOMETER_PIN A6

#define BDAY_LED_PIN A0
#define CLOCK_LED_PIN A1
#define TIME_LED_PIN A2
#define DATE_LED_PIN A3

#define LED_PIN 13

#define BDAY_BTN_PIN 2
#define CLOCK_BTN_PIN 5
#define TIME_BTN_PIN 3
#define DATE_BTN_PIN 4

#define MIN_BRIGHTNESS 2

// Patterns for common anode 7-segment displays
#define CA_L B11100011
#define CA_I B11110011
#define CA_F B01110001
#define CA_E B01100001
#define CA_C B01100011
#define CA_O_DP B00000010
#define CA_DASH B11111101
#define CA_FULL B00000000


#endif
