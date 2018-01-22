
#include <Led.h>

#ifndef LEDLIST_H
#define LEDLIST_H

class LedList {
  int leds;
  Led* led[20];

public:
  LedList();
  void registerLed( Led* led);
  void update();
};

#endif
