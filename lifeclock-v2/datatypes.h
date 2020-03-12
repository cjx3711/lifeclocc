#ifndef DATATYPES_H
#define DATATYPES_H

#include <Wire.h>
#include <avr/eeprom.h>

struct SDate {
  uint16_t date;
  uint16_t month;
  uint16_t year;
};

struct STime {
  uint16_t hour;
  uint16_t minute;
  uint16_t second;
};

#endif
