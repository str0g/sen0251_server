#include "listners.h"
int ListnerTemperature::get_reading() {
  buf[0] = '3';
  buf[1] = '2';
  buf[2] = '.';
  buf[3] = '0';
  buf[4] = 'C';
  return 5;
}

int ListnerPressure::get_reading() {
  return 0;
}

int ListnerAltitiude::get_reading() {
  return 0;
}
