#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SECONDS_IN_DAY 86400

#define SECONDS false
#define DAYS true

//Pin connected to ST_CP of 74HC595
#define SR_LATCH_PIN 8
//Pin connected to SH_CP of 74HC595
#define SR_CLOCK_PIN 7
////Pin connected to DS of 74HC595
#define SR_DATA_PIN 9

#define DSP_POWER_PIN 6
#define POTIOMETER_PIN A6

#define LED_BDAY_PIN A0
#define LED_PREV_PIN A0
#define LED_CLOCK_PIN A2
#define LED_NEXT_PIN A2
#define LED_UP_PIN A3
#define LED_TIME_PIN A3
#define LED_DOWN_PIN A1
#define LED_DATE_PIN A1

#define LED_PIN 13

#define BTN_BDAY_PIN 2
#define BTN_PREV_PIN 2
#define BTN_CLOCK_PIN 3
#define BTN_NEXT_PIN 3
#define BTN_UP_PIN 4
#define BTN_TIME_PIN 4
#define BTN_DOWN_PIN 5
#define BTN_DATE_PIN 5

#define MIN_BRIGHTNESS 2

#define BTN_UP 0
#define BTN_TIME 0
#define BTN_DOWN 1
#define BTN_DATE 1
#define BTN_PREV 2
#define BTN_BDAY 2
#define BTN_NEXT 3
#define BTN_CLOCK 3

// Patterns for common anode 7-segment displays
#define CA_L B11100011
#define CA_I B11110011
#define CA_F B01110001
#define CA_E B01100001
#define CA_C B01100011
#define CA_O_DP B00000010
#define CA_DASH B11111101
#define CA_FULL B00000000
#define CA_BLANK B11111111
#define CA_DOT B11111110

#define CA_LOWER_LEFT_C B11100101
#define CA_LOWER_RIGHT_C B11001101
#define CA_LOWER_O B11000101
#define CA_LOWER_DASH B11101111
#define CA_LOWER_RIGHT B11110111
#define CA_LOWER_LEFT B11011111

#define SHORT_PRESS_TIMEOUT 800
#define LONG_PRESS_TIMEOUT 1200
#define SET_STATE_TIMEOUT 15000
#define CLOCK_STATE_TIMEOUT 5000
#define BLINK_MS 250
#define REPEAT_MS 100


#define STATE_CLOCK 0
#define STATE_SET_CLOCK 1
#define STATE_SET_BIRTHDAY 2
#define STATE_DEBUG 3
#define STATE_GAME 4

#endif
