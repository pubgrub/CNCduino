#include <LedList.h>

LedList::LedList() {
  leds = 0;
}

void LedList::registerLed( Led *l) {
  led[ leds] = l;
  leds++;
}

void LedList::update() {
  for( int i = 0; i < leds; i++) {
    led[ i]->update();
  }
}
