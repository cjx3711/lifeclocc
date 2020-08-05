#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SECONDS 0
#define DAYS 1
#define WEEKS 2


// ====== SETTINGS ======
// If it's common cathode, set to 0
#define COMMON_ANODE 1
// Display mode can either be SECONDS, DAYS or WEEKS
#define DISPLAY_MODE DAYS
// Years to count down from
#define TOTAL_YEARS 80
// Debug mode will print stuff
#define DEBUG true

#define SPLIT_DISPLAY 0
// ====== SETTINGS ======

#define SECONDS_IN_DAY 86400
#define DAYS_IN_WEEK 7



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
#define CA_L 0b11100011
#define CA_I 0b11110011
#define CA_F 0b01110001
#define CA_E 0b01100001
#define CA_C 0b01100011
#define CA_O_DP 0b00000010
#define CA_O 0b00000011
#define CA_N 0b00010011

#define CA_DASH 0b11111101
#define CA_FULL 0b00000000
#define CA_BLANK 0b11111111
#define CA_DOT 0b11111110

#define CA_LOWER_LEFT_C 0b11100101
#define CA_LOWER_RIGHT_C 0b11001101
#define CA_LOWER_O 0b11000101
#define CA_LOWER_DASH 0b11101111
#define CA_LOWER_RIGHT 0b11110111
#define CA_LOWER_LEFT 0b11011111

#define CA_SEG_A 0b01111111
#define CA_SEG_B 0b10111111
#define CA_SEG_C 0b11011111
#define CA_SEG_D 0b11101111
#define CA_SEG_E 0b11110111
#define CA_SEG_F 0b11111011
#define CA_SEG_G 0b11111101
#define CA_SEG_DP 0b11111110

#define CA_0 0b11111100
#define CA_1 0b01100000
#define CA_2 0b11011010
#define CA_3 0b11110010
#define CA_4 0b01100110
#define CA_5 0b10110110
#define CA_6 0b10111110
#define CA_7 0b11100000
#define CA_8 0b11111110
#define CA_9 0b11110110

#define CA_0_DP 0b11111101
#define CA_1_DP 0b01100001
#define CA_2_DP 0b11011011
#define CA_3_DP 0b11110011
#define CA_4_DP 0b01100111
#define CA_5_DP 0b10110111
#define CA_6_DP 0b10111111
#define CA_7_DP 0b11100001
#define CA_8_DP 0b11111111
#define CA_9_DP 0b11110111

#if COMMON_ANODE

#define DSP(SYM) (CA_##SYM)

#else

#define DSP(SYM) ((CA_##SYM) ^ 0b11111111 )

#endif


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
