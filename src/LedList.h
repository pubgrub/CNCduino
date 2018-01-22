#ifndef LEDLIST_H
#define LEDLIST_H

class Led;

class LedList {
  int leds;
  Led* led[20];

public:
  LedList();
  void registerLed( Led* led);
  void update();
};

#include <Led.h>


#endif
