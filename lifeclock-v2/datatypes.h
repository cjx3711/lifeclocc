#ifndef DATATYPES_H
#define DATATYPES_H

#include <Wire.h>
#include <avr/eeprom.h>

struct SDate {
  uint16_t date;
  uint16_t month;
  uint16_t year;
};


#endif
